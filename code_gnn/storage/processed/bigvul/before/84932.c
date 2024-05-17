assemble_neg_contexts(struct smb2_negotiate_req *req)
{

	 
	char *pneg_ctxt = (char *)req + OFFSET_OF_NEG_CONTEXT + 4;

	build_preauth_ctxt((struct smb2_preauth_neg_context *)pneg_ctxt);
	 
	pneg_ctxt += 2 + sizeof(struct smb2_preauth_neg_context);
	build_encrypt_ctxt((struct smb2_encryption_neg_context *)pneg_ctxt);
	req->NegotiateContextOffset = cpu_to_le32(OFFSET_OF_NEG_CONTEXT);
	req->NegotiateContextCount = cpu_to_le16(2);
	inc_rfc1001_len(req, 4 + sizeof(struct smb2_preauth_neg_context) + 2
			+ sizeof(struct smb2_encryption_neg_context));  
}
