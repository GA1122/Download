static int PC4500_readrid(struct airo_info *ai, u16 rid, void *pBuf, int len, int lock)
{
	u16 status;
        int rc = SUCCESS;

	if (lock) {
		if (down_interruptible(&ai->sem))
			return ERROR;
	}
	if (test_bit(FLAG_MPI,&ai->flags)) {
		Cmd cmd;
		Resp rsp;

		memset(&cmd, 0, sizeof(cmd));
		memset(&rsp, 0, sizeof(rsp));
		ai->config_desc.rid_desc.valid = 1;
		ai->config_desc.rid_desc.len = RIDSIZE;
		ai->config_desc.rid_desc.rid = 0;
		ai->config_desc.rid_desc.host_addr = ai->ridbus;

		cmd.cmd = CMD_ACCESS;
		cmd.parm0 = rid;

		memcpy_toio(ai->config_desc.card_ram_off,
			&ai->config_desc.rid_desc, sizeof(Rid));

		rc = issuecommand(ai, &cmd, &rsp);

		if (rsp.status & 0x7f00)
			rc = rsp.rsp0;
		if (!rc)
			memcpy(pBuf, ai->config_desc.virtual_host_addr, len);
		goto done;
	} else {
		if ((status = PC4500_accessrid(ai, rid, CMD_ACCESS))!=SUCCESS) {
	                rc = status;
	                goto done;
	        }
		if (bap_setup(ai, rid, 0, BAP1) != SUCCESS) {
			rc = ERROR;
	                goto done;
	        }
		bap_read(ai, pBuf, 2, BAP1);
		len = min(len, (int)le16_to_cpu(*(__le16*)pBuf)) - 2;

		if ( len <= 2 ) {
			airo_print_err(ai->dev->name,
				"Rid %x has a length of %d which is too short",
				(int)rid, (int)len );
			rc = ERROR;
	                goto done;
		}
		rc = bap_read(ai, ((__le16*)pBuf)+1, len, BAP1);
	}
done:
	if (lock)
		up(&ai->sem);
	return rc;
}