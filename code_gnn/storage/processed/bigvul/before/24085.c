static int transmit_802_11_packet(struct airo_info *ai, int len, char *pPacket)
{
	__le16 fc, payloadLen;
	Cmd cmd;
	Resp rsp;
	int hdrlen;
	static u8 tail[(30-10) + 2 + 6] = {[30-10] = 6};
	 
	u16 txFid = len;
	len >>= 16;

	fc = *(__le16*)pPacket;
	hdrlen = header_len(fc);

	if (len < hdrlen) {
		airo_print_warn(ai->dev->name, "Short packet %d", len);
		return ERROR;
	}

	 
	if (bap_setup(ai, txFid, 6, BAP1) != SUCCESS) return ERROR;
	 
	payloadLen = cpu_to_le16(len-hdrlen);
	bap_write(ai, &payloadLen, sizeof(payloadLen),BAP1);
	if (bap_setup(ai, txFid, 0x0014, BAP1) != SUCCESS) return ERROR;
	bap_write(ai, (__le16 *)pPacket, hdrlen, BAP1);
	bap_write(ai, (__le16 *)(tail + (hdrlen - 10)), 38 - hdrlen, BAP1);

	bap_write(ai, (__le16 *)(pPacket + hdrlen), len - hdrlen, BAP1);
	memset( &cmd, 0, sizeof( cmd ) );
	cmd.cmd = CMD_TRANSMIT;
	cmd.parm0 = txFid;
	if (issuecommand(ai, &cmd, &rsp) != SUCCESS) return ERROR;
	if ( (rsp.status & 0xFF00) != 0) return ERROR;
	return SUCCESS;
}
