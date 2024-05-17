static int vhost_scsi_write_pending(struct se_cmd *se_cmd)
{
	 
	target_execute_cmd(se_cmd);
	return 0;
}
