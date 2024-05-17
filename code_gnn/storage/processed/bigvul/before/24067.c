static int readBSSListRid(struct airo_info *ai, int first,
		      BSSListRid *list)
{
	Cmd cmd;
	Resp rsp;

	if (first == 1) {
		if (ai->flags & FLAG_RADIO_MASK) return -ENETDOWN;
		memset(&cmd, 0, sizeof(cmd));
		cmd.cmd=CMD_LISTBSS;
		if (down_interruptible(&ai->sem))
			return -ERESTARTSYS;
		ai->list_bss_task = current;
		issuecommand(ai, &cmd, &rsp);
		up(&ai->sem);
		 
		schedule_timeout_uninterruptible(3 * HZ);
		ai->list_bss_task = NULL;
	}
	return PC4500_readrid(ai, first ? ai->bssListFirst : ai->bssListNext,
			    list, ai->bssListRidLen, 1);
}