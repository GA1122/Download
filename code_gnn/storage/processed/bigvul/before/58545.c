static int peer_recv_fastpath_pdu(freerdp_peer* client, wStream* s)
{
	rdpRdp* rdp;
	UINT16 length;
	rdpFastPath* fastpath;

	rdp = client->context->rdp;
	fastpath = rdp->fastpath;

	fastpath_read_header_rdp(fastpath, s, &length);

	if ((length == 0) || (length > Stream_GetRemainingLength(s)))
	{
		fprintf(stderr, "incorrect FastPath PDU header length %d\n", length);
		return -1;
	}

	if (fastpath->encryptionFlags & FASTPATH_OUTPUT_ENCRYPTED)
	{
		if (!rdp_decrypt(rdp, s, length, (fastpath->encryptionFlags & FASTPATH_OUTPUT_SECURE_CHECKSUM) ? SEC_SECURE_CHECKSUM : 0))
			return -1;
	}

	return fastpath_recv_inputs(fastpath, s);
}