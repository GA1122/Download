 int build_ntlmssp_auth_blob(unsigned char **pbuffer,
 					u16 *buflen,
 				   struct cifs_ses *ses,
 				   const struct nls_table *nls_cp)
 {
 	int rc;
 	AUTHENTICATE_MESSAGE *sec_blob;
 	__u32 flags;
 	unsigned char *tmp;
 
 	rc = setup_ntlmv2_rsp(ses, nls_cp);
 	if (rc) {
 		cifs_dbg(VFS, "Error %d during NTLMSSP authentication\n", rc);
 		*buflen = 0;
 		goto setup_ntlmv2_ret;
 	}
 	*pbuffer = kmalloc(size_of_ntlmssp_blob(ses), GFP_KERNEL);
 	sec_blob = (AUTHENTICATE_MESSAGE *)*pbuffer;
 
 	memcpy(sec_blob->Signature, NTLMSSP_SIGNATURE, 8);
 	sec_blob->MessageType = NtLmAuthenticate;
 
  	flags = NTLMSSP_NEGOTIATE_56 |
  		NTLMSSP_REQUEST_TARGET | NTLMSSP_NEGOTIATE_TARGET_INFO |
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
  
  	tmp = *pbuffer + sizeof(AUTHENTICATE_MESSAGE);
  	sec_blob->NegotiateFlags = cpu_to_le32(flags);
 
 	sec_blob->LmChallengeResponse.BufferOffset =
 				cpu_to_le32(sizeof(AUTHENTICATE_MESSAGE));
 	sec_blob->LmChallengeResponse.Length = 0;
 	sec_blob->LmChallengeResponse.MaximumLength = 0;
 
 	sec_blob->NtChallengeResponse.BufferOffset =
 				cpu_to_le32(tmp - *pbuffer);
 	if (ses->user_name != NULL) {
 		memcpy(tmp, ses->auth_key.response + CIFS_SESS_KEY_SIZE,
 				ses->auth_key.len - CIFS_SESS_KEY_SIZE);
 		tmp += ses->auth_key.len - CIFS_SESS_KEY_SIZE;
 
 		sec_blob->NtChallengeResponse.Length =
 				cpu_to_le16(ses->auth_key.len - CIFS_SESS_KEY_SIZE);
 		sec_blob->NtChallengeResponse.MaximumLength =
 				cpu_to_le16(ses->auth_key.len - CIFS_SESS_KEY_SIZE);
 	} else {
 		 
 		sec_blob->NtChallengeResponse.Length = 0;
 		sec_blob->NtChallengeResponse.MaximumLength = 0;
 	}
 
 	if (ses->domainName == NULL) {
 		sec_blob->DomainName.BufferOffset = cpu_to_le32(tmp - *pbuffer);
 		sec_blob->DomainName.Length = 0;
 		sec_blob->DomainName.MaximumLength = 0;
 		tmp += 2;
 	} else {
 		int len;
 		len = cifs_strtoUTF16((__le16 *)tmp, ses->domainName,
 				      CIFS_MAX_DOMAINNAME_LEN, nls_cp);
 		len *= 2;  
 		sec_blob->DomainName.BufferOffset = cpu_to_le32(tmp - *pbuffer);
 		sec_blob->DomainName.Length = cpu_to_le16(len);
 		sec_blob->DomainName.MaximumLength = cpu_to_le16(len);
 		tmp += len;
 	}
 
 	if (ses->user_name == NULL) {
 		sec_blob->UserName.BufferOffset = cpu_to_le32(tmp - *pbuffer);
 		sec_blob->UserName.Length = 0;
 		sec_blob->UserName.MaximumLength = 0;
 		tmp += 2;
 	} else {
 		int len;
 		len = cifs_strtoUTF16((__le16 *)tmp, ses->user_name,
 				      CIFS_MAX_USERNAME_LEN, nls_cp);
 		len *= 2;  
 		sec_blob->UserName.BufferOffset = cpu_to_le32(tmp - *pbuffer);
 		sec_blob->UserName.Length = cpu_to_le16(len);
 		sec_blob->UserName.MaximumLength = cpu_to_le16(len);
 		tmp += len;
 	}
 
 	sec_blob->WorkstationName.BufferOffset = cpu_to_le32(tmp - *pbuffer);
 	sec_blob->WorkstationName.Length = 0;
 	sec_blob->WorkstationName.MaximumLength = 0;
 	tmp += 2;
 
 	if (((ses->ntlmssp->server_flags & NTLMSSP_NEGOTIATE_KEY_XCH) ||
 		(ses->ntlmssp->server_flags & NTLMSSP_NEGOTIATE_EXTENDED_SEC))
 			&& !calc_seckey(ses)) {
 		memcpy(tmp, ses->ntlmssp->ciphertext, CIFS_CPHTXT_SIZE);
 		sec_blob->SessionKey.BufferOffset = cpu_to_le32(tmp - *pbuffer);
 		sec_blob->SessionKey.Length = cpu_to_le16(CIFS_CPHTXT_SIZE);
 		sec_blob->SessionKey.MaximumLength =
 				cpu_to_le16(CIFS_CPHTXT_SIZE);
 		tmp += CIFS_CPHTXT_SIZE;
 	} else {
 		sec_blob->SessionKey.BufferOffset = cpu_to_le32(tmp - *pbuffer);
 		sec_blob->SessionKey.Length = 0;
 		sec_blob->SessionKey.MaximumLength = 0;
 	}
 
 	*buflen = tmp - *pbuffer;
 setup_ntlmv2_ret:
 	return rc;
 }