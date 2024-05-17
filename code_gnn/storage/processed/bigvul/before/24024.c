static int enable_MAC(struct airo_info *ai, int lock)
{
	int rc;
	Cmd cmd;
	Resp rsp;

	 
	if (ai->flags & FLAG_RADIO_MASK) return SUCCESS;

	if (lock && down_interruptible(&ai->sem))
		return -ERESTARTSYS;

	if (!test_bit(FLAG_ENABLED, &ai->flags)) {
		memset(&cmd, 0, sizeof(cmd));
		cmd.cmd = MAC_ENABLE;
		rc = issuecommand(ai, &cmd, &rsp);
		if (rc == SUCCESS)
			set_bit(FLAG_ENABLED, &ai->flags);
	} else
		rc = SUCCESS;

	if (lock)
	    up(&ai->sem);

	if (rc)
		airo_print_err(ai->dev->name, "Cannot enable MAC");
	else if ((rsp.status & 0xFF00) != 0) {
		airo_print_err(ai->dev->name, "Bad MAC enable reason=%x, "
			"rid=%x, offset=%d", rsp.rsp0, rsp.rsp1, rsp.rsp2);
		rc = ERROR;
	}
	return rc;
}
