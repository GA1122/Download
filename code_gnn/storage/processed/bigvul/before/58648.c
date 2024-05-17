void rdp_write_header(rdpRdp* rdp, STREAM* s, UINT16 length, UINT16 channel_id)
{
	int body_length;
	enum DomainMCSPDU MCSPDU;

	MCSPDU = (rdp->settings->ServerMode) ? DomainMCSPDU_SendDataIndication : DomainMCSPDU_SendDataRequest;

	if ((rdp->sec_flags & SEC_ENCRYPT) && (rdp->settings->EncryptionMethods == ENCRYPTION_METHOD_FIPS))
	{
		int pad;

		body_length = length - RDP_PACKET_HEADER_MAX_LENGTH - 16;
		pad = 8 - (body_length % 8);

		if (pad != 8)
			length += pad;
	}

	mcs_write_domain_mcspdu_header(s, MCSPDU, length, 0);
	per_write_integer16(s, rdp->mcs->user_id, MCS_BASE_CHANNEL_ID);  
	per_write_integer16(s, channel_id, 0);  
	stream_write_BYTE(s, 0x70);  
	 
	length = (length - RDP_PACKET_HEADER_MAX_LENGTH) | 0x8000;
	stream_write_UINT16_be(s, length);  
}