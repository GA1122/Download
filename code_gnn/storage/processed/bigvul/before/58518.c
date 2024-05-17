SECURITY_STATUS credssp_encrypt_ts_credentials(rdpCredssp* credssp)
{
	SecBuffer Buffers[2];
	SecBufferDesc Message;
	SECURITY_STATUS status;

	credssp_encode_ts_credentials(credssp);

	Buffers[0].BufferType = SECBUFFER_TOKEN;  
	Buffers[1].BufferType = SECBUFFER_DATA;  

	sspi_SecBufferAlloc(&credssp->authInfo, credssp->ContextSizes.cbMaxSignature + credssp->ts_credentials.cbBuffer);

	Buffers[0].cbBuffer = credssp->ContextSizes.cbMaxSignature;
	Buffers[0].pvBuffer = credssp->authInfo.pvBuffer;
	ZeroMemory(Buffers[0].pvBuffer, Buffers[0].cbBuffer);

	Buffers[1].cbBuffer = credssp->ts_credentials.cbBuffer;
	Buffers[1].pvBuffer = &((BYTE*) credssp->authInfo.pvBuffer)[Buffers[0].cbBuffer];
	CopyMemory(Buffers[1].pvBuffer, credssp->ts_credentials.pvBuffer, Buffers[1].cbBuffer);

	Message.cBuffers = 2;
	Message.ulVersion = SECBUFFER_VERSION;
	Message.pBuffers = (PSecBuffer) &Buffers;

	status = credssp->table->EncryptMessage(&credssp->context, 0, &Message, credssp->send_seq_num++);

	if (status != SEC_E_OK)
		return status;

	return SEC_E_OK;
}