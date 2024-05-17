static UINT32 rdp_security_stream_out(rdpRdp* rdp, STREAM* s, int length)
{
	BYTE* data;
	UINT32 sec_flags;
	UINT32 pad = 0;

	sec_flags = rdp->sec_flags;

	if (sec_flags != 0)
	{
		rdp_write_security_header(s, sec_flags);

		if (sec_flags & SEC_ENCRYPT)
		{
			if (rdp->settings->EncryptionMethods == ENCRYPTION_METHOD_FIPS)
			{
				data = s->p + 12;

				length = length - (data - s->data);
				stream_write_UINT16(s, 0x10);  
				stream_write_BYTE(s, 0x1);  

				 
				pad = 8 - (length % 8);

				if (pad == 8)
					pad = 0;
				if (pad)
					memset(data+length, 0, pad);

				stream_write_BYTE(s, pad);

				security_hmac_signature(data, length, s->p, rdp);
				stream_seek(s, 8);
				security_fips_encrypt(data, length + pad, rdp);
			}
			else
			{
				data = s->p + 8;
				length = length - (data - s->data);
				if (sec_flags & SEC_SECURE_CHECKSUM)
					security_salted_mac_signature(rdp, data, length, TRUE, s->p);
				else
					security_mac_signature(rdp, data, length, s->p);
				stream_seek(s, 8);
				security_encrypt(s->p, length, rdp);
			}
		}

		rdp->sec_flags = 0;
	}

	return pad;
}