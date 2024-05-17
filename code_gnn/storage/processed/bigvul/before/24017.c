static void disable_MAC( struct airo_info *ai, int lock ) {
        Cmd cmd;
	Resp rsp;

	if (lock && down_interruptible(&ai->sem))
		return;

	if (test_bit(FLAG_ENABLED, &ai->flags)) {
		memset(&cmd, 0, sizeof(cmd));
		cmd.cmd = MAC_DISABLE;  
		issuecommand(ai, &cmd, &rsp);
		clear_bit(FLAG_ENABLED, &ai->flags);
	}
	if (lock)
		up(&ai->sem);
}