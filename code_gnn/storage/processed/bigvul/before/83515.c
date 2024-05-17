SECURITY_STATUS ntlm_server_AuthenticateComplete(NTLM_CONTEXT* context)
{
	UINT32 flags = 0;
	NTLM_AV_PAIR* AvFlags = NULL;
	NTLM_AUTHENTICATE_MESSAGE* message;
	BYTE messageIntegrityCheck[16];

	if (context->state != NTLM_STATE_COMPLETION)
		return SEC_E_OUT_OF_SEQUENCE;

	message = &context->AUTHENTICATE_MESSAGE;
	AvFlags = ntlm_av_pair_get(context->NTLMv2Response.Challenge.AvPairs, MsvAvFlags);

	if (AvFlags)
		Data_Read_UINT32(ntlm_av_pair_get_value_pointer(AvFlags), flags);

	if (ntlm_compute_lm_v2_response(context) < 0)  
		return SEC_E_INTERNAL_ERROR;

	if (ntlm_compute_ntlm_v2_response(context) < 0)  
		return SEC_E_INTERNAL_ERROR;

	 
	ntlm_generate_key_exchange_key(context);
	 
	ntlm_decrypt_random_session_key(context);
	 
	ntlm_generate_exported_session_key(context);

	if (flags & MSV_AV_FLAGS_MESSAGE_INTEGRITY_CHECK)
	{
		ZeroMemory(&((PBYTE) context->AuthenticateMessage.pvBuffer)[context->MessageIntegrityCheckOffset],
		           16);
		ntlm_compute_message_integrity_check(context, messageIntegrityCheck, sizeof(messageIntegrityCheck));
		CopyMemory(&((PBYTE) context->AuthenticateMessage.pvBuffer)[context->MessageIntegrityCheckOffset],
		           message->MessageIntegrityCheck, 16);

		if (memcmp(messageIntegrityCheck, message->MessageIntegrityCheck, 16) != 0)
		{
			WLog_ERR(TAG, "Message Integrity Check (MIC) verification failed!");
			WLog_ERR(TAG, "Expected MIC:");
			winpr_HexDump(TAG, WLOG_ERROR, messageIntegrityCheck, 16);
			WLog_ERR(TAG, "Actual MIC:");
			winpr_HexDump(TAG, WLOG_ERROR, message->MessageIntegrityCheck, 16);
			return SEC_E_MESSAGE_ALTERED;
		}
	}

	 
	ntlm_generate_client_signing_key(context);
	ntlm_generate_server_signing_key(context);
	 
	ntlm_generate_client_sealing_key(context);
	ntlm_generate_server_sealing_key(context);
	 
	ntlm_init_rc4_seal_states(context);
#ifdef WITH_DEBUG_NTLM
	WLog_DBG(TAG, "ClientChallenge");
	winpr_HexDump(TAG, WLOG_DEBUG, context->ClientChallenge, 8);
	WLog_DBG(TAG, "ServerChallenge");
	winpr_HexDump(TAG, WLOG_DEBUG, context->ServerChallenge, 8);
	WLog_DBG(TAG, "SessionBaseKey");
	winpr_HexDump(TAG, WLOG_DEBUG, context->SessionBaseKey, 16);
	WLog_DBG(TAG, "KeyExchangeKey");
	winpr_HexDump(TAG, WLOG_DEBUG, context->KeyExchangeKey, 16);
	WLog_DBG(TAG, "ExportedSessionKey");
	winpr_HexDump(TAG, WLOG_DEBUG, context->ExportedSessionKey, 16);
	WLog_DBG(TAG, "RandomSessionKey");
	winpr_HexDump(TAG, WLOG_DEBUG, context->RandomSessionKey, 16);
	WLog_DBG(TAG, "ClientSigningKey");
	winpr_HexDump(TAG, WLOG_DEBUG, context->ClientSigningKey, 16);
	WLog_DBG(TAG, "ClientSealingKey");
	winpr_HexDump(TAG, WLOG_DEBUG, context->ClientSealingKey, 16);
	WLog_DBG(TAG, "ServerSigningKey");
	winpr_HexDump(TAG, WLOG_DEBUG, context->ServerSigningKey, 16);
	WLog_DBG(TAG, "ServerSealingKey");
	winpr_HexDump(TAG, WLOG_DEBUG, context->ServerSealingKey, 16);
	WLog_DBG(TAG, "Timestamp");
	winpr_HexDump(TAG, WLOG_DEBUG, context->Timestamp, 8);
#endif
	context->state = NTLM_STATE_FINAL;
	ntlm_free_message_fields_buffer(&(message->DomainName));
	ntlm_free_message_fields_buffer(&(message->UserName));
	ntlm_free_message_fields_buffer(&(message->Workstation));
	ntlm_free_message_fields_buffer(&(message->LmChallengeResponse));
	ntlm_free_message_fields_buffer(&(message->NtChallengeResponse));
	ntlm_free_message_fields_buffer(&(message->EncryptedRandomSessionKey));
	return SEC_E_OK;
}