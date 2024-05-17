static int PC4500_writerid(struct airo_info *ai, u16 rid,
			   const void *pBuf, int len, int lock)
{
	u16 status;
	int rc = SUCCESS;

	*(__le16*)pBuf = cpu_to_le16((u16)len);

	if (lock) {
		if (down_interruptible(&ai->sem))
			return ERROR;
	}
	if (test_bit(FLAG_MPI,&ai->flags)) {
		Cmd cmd;
		Resp rsp;

		if (test_bit(FLAG_ENABLED, &ai->flags) && (RID_WEP_TEMP != rid))
			airo_print_err(ai->dev->name,
				"%s: MAC should be disabled (rid=%04x)",
				__func__, rid);
		memset(&cmd, 0, sizeof(cmd));
		memset(&rsp, 0, sizeof(rsp));

		ai->config_desc.rid_desc.valid = 1;
		ai->config_desc.rid_desc.len = *((u16 *)pBuf);
		ai->config_desc.rid_desc.rid = 0;

		cmd.cmd = CMD_WRITERID;
		cmd.parm0 = rid;

		memcpy_toio(ai->config_desc.card_ram_off,
			&ai->config_desc.rid_desc, sizeof(Rid));

		if (len < 4 || len > 2047) {
			airo_print_err(ai->dev->name, "%s: len=%d", __func__, len);
			rc = -1;
		} else {
			memcpy((char *)ai->config_desc.virtual_host_addr,
				pBuf, len);

			rc = issuecommand(ai, &cmd, &rsp);
			if ((rc & 0xff00) != 0) {
				airo_print_err(ai->dev->name, "%s: Write rid Error %d",
						__func__, rc);
				airo_print_err(ai->dev->name, "%s: Cmd=%04x",
						__func__, cmd.cmd);
			}

			if ((rsp.status & 0x7f00))
				rc = rsp.rsp0;
		}
	} else {
		if ( (status = PC4500_accessrid(ai, rid, CMD_ACCESS)) != 0) {
	                rc = status;
	                goto done;
	        }
		if (bap_setup(ai, rid, 0, BAP1) != SUCCESS) {
	                rc = ERROR;
	                goto done;
	        }
		bap_write(ai, pBuf, len, BAP1);
		rc = PC4500_accessrid(ai, rid, 0x100|CMD_ACCESS);
	}
done:
	if (lock)
		up(&ai->sem);
        return rc;
}