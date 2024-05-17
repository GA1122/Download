 void build_ntlmssp_negotiate_blob(unsigned char *pbuffer,
 					 struct cifs_ses *ses)
 {
 	NEGOTIATE_MESSAGE *sec_blob = (NEGOTIATE_MESSAGE *)pbuffer;
 	__u32 flags;
 
 	memset(pbuffer, 0, sizeof(NEGOTIATE_MESSAGE));
 	memcpy(sec_blob->Signature, NTLMSSP_SIGNATURE, 8);
 	sec_blob->MessageType = NtLmNegotiate;
 
  	 
  	flags = NTLMSSP_NEGOTIATE_56 |	NTLMSSP_REQUEST_TARGET |
  		NTLMSSP_NEGOTIATE_128 | NTLMSSP_NEGOTIATE_UNICODE |
		NTLMSSP_NEGOTIATE_NTLM | NTLMSSP_NEGOTIATE_EXTENDED_SEC;
	if (ses->server->sign) {
// 		NTLMSSP_NEGOTIATE_NTLM | NTLMSSP_NEGOTIATE_EXTENDED_SEC |
// 		NTLMSSP_NEGOTIATE_SEAL;
// 	if (ses->server->sign)
  		flags |= NTLMSSP_NEGOTIATE_SIGN;
		if (!ses->server->session_estab ||
				ses->ntlmssp->sesskey_per_smbsess)
			flags |= NTLMSSP_NEGOTIATE_KEY_XCH;
	}
// 	if (!ses->server->session_estab || ses->ntlmssp->sesskey_per_smbsess)
// 		flags |= NTLMSSP_NEGOTIATE_KEY_XCH;
  
  	sec_blob->NegotiateFlags = cpu_to_le32(flags);
  
 	sec_blob->WorkstationName.BufferOffset = 0;
 	sec_blob->WorkstationName.Length = 0;
 	sec_blob->WorkstationName.MaximumLength = 0;
 
 	 
 	sec_blob->DomainName.BufferOffset = 0;
 	sec_blob->DomainName.Length = 0;
 	sec_blob->DomainName.MaximumLength = 0;
 }