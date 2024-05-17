static int PC4500_accessrid(struct airo_info *ai, u16 rid, u16 accmd)
{
	Cmd cmd;  
	Resp rsp;  
	u16 status;

	memset(&cmd, 0, sizeof(cmd));
	cmd.cmd = accmd;
	cmd.parm0 = rid;
	status = issuecommand(ai, &cmd, &rsp);
	if (status != 0) return status;
	if ( (rsp.status & 0x7F00) != 0) {
		return (accmd << 8) + (rsp.rsp0 & 0xFF);
	}
	return 0;
}