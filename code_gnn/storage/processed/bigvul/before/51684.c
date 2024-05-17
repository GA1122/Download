void ikev1_echo_hdr(struct msg_digest *md, bool enc, u_int8_t np)
{
	struct isakmp_hdr hdr = md->hdr;  

	 
	init_out_pbs(&reply_stream, reply_buffer, sizeof(reply_buffer),
		 "reply packet");

	hdr.isa_flags = 0;  
	if (enc)
		hdr.isa_flags |= ISAKMP_FLAGS_v1_ENCRYPTION;

	if (DBGP(IMPAIR_SEND_BOGUS_ISAKMP_FLAG)) {
		hdr.isa_flags |= ISAKMP_FLAGS_RESERVED_BIT6;
	}

	 
	hdr.isa_np = np;
	if (!out_struct(&hdr, &isakmp_hdr_desc, &reply_stream, &md->rbody))
		impossible();  
}
