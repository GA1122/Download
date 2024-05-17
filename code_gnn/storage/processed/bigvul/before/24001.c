static int airo_set_scan(struct net_device *dev,
			 struct iw_request_info *info,
			 struct iw_point *dwrq,
			 char *extra)
{
	struct airo_info *ai = dev->ml_priv;
	Cmd cmd;
	Resp rsp;
	int wake = 0;

	 
	if (ai->flags & FLAG_RADIO_MASK) return -ENETDOWN;

	if (down_interruptible(&ai->sem))
		return -ERESTARTSYS;

	 
	if (ai->scan_timeout > 0)
		goto out;

	 
	ai->scan_timeout = RUN_AT(3*HZ);
	memset(&cmd, 0, sizeof(cmd));
	cmd.cmd=CMD_LISTBSS;
	issuecommand(ai, &cmd, &rsp);
	wake = 1;

out:
	up(&ai->sem);
	if (wake)
		wake_up_interruptible(&ai->thr_wait);
	return 0;
}
