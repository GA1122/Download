static int transmit_802_3_packet(struct airo_info *ai, int len, char *pPacket)
{
	__le16 payloadLen;
	Cmd cmd;
	Resp rsp;
	int miclen = 0;
	u16 txFid = len;
	MICBuffer pMic;

	len >>= 16;

	if (len <= ETH_ALEN * 2) {
		airo_print_warn(ai->dev->name, "Short packet %d", len);
		return ERROR;
	}
	len -= ETH_ALEN * 2;

	if (test_bit(FLAG_MIC_CAPABLE, &ai->flags) && ai->micstats.enabled && 
	    (ntohs(((__be16 *)pPacket)[6]) != 0x888E)) {
		if (encapsulate(ai,(etherHead *)pPacket,&pMic,len) != SUCCESS)
			return ERROR;
		miclen = sizeof(pMic);
	}
	if (bap_setup(ai, txFid, 0x0036, BAP1) != SUCCESS) return ERROR;
	 
	payloadLen = cpu_to_le16(len + miclen);
	bap_write(ai, &payloadLen, sizeof(payloadLen),BAP1);
	bap_write(ai, (__le16*)pPacket, sizeof(etherHead), BAP1);
	if (miclen)
		bap_write(ai, (__le16*)&pMic, miclen, BAP1);
	bap_write(ai, (__le16*)(pPacket + sizeof(etherHead)), len, BAP1);
	memset( &cmd, 0, sizeof( cmd ) );
	cmd.cmd = CMD_TRANSMIT;
	cmd.parm0 = txFid;
	if (issuecommand(ai, &cmd, &rsp) != SUCCESS) return ERROR;
	if ( (rsp.status & 0xFF00) != 0) return ERROR;
	return SUCCESS;
}