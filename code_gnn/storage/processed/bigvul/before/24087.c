static u16 transmit_allocate(struct airo_info *ai, int lenPayload, int raw)
{
	unsigned int loop = 3000;
	Cmd cmd;
	Resp rsp;
	u16 txFid;
	__le16 txControl;

	cmd.cmd = CMD_ALLOCATETX;
	cmd.parm0 = lenPayload;
	if (down_interruptible(&ai->sem))
		return ERROR;
	if (issuecommand(ai, &cmd, &rsp) != SUCCESS) {
		txFid = ERROR;
		goto done;
	}
	if ( (rsp.status & 0xFF00) != 0) {
		txFid = ERROR;
		goto done;
	}
	 
	while (((IN4500(ai, EVSTAT) & EV_ALLOC) == 0) && --loop);
	if (!loop) {
		txFid = ERROR;
		goto done;
	}

	txFid = IN4500(ai, TXALLOCFID);
	OUT4500(ai, EVACK, EV_ALLOC);

	 
	 
	if (raw)
		txControl = cpu_to_le16(TXCTL_TXOK | TXCTL_TXEX | TXCTL_802_11
			| TXCTL_ETHERNET | TXCTL_NORELEASE);
	else
		txControl = cpu_to_le16(TXCTL_TXOK | TXCTL_TXEX | TXCTL_802_3
			| TXCTL_ETHERNET | TXCTL_NORELEASE);
	if (bap_setup(ai, txFid, 0x0008, BAP1) != SUCCESS)
		txFid = ERROR;
	else
		bap_write(ai, &txControl, sizeof(txControl), BAP1);

done:
	up(&ai->sem);

	return txFid;
}
