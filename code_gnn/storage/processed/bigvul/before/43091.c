static void vhost_scsi_free_cmd(struct vhost_scsi_cmd *cmd)
{
	struct se_cmd *se_cmd = &cmd->tvc_se_cmd;

	 
	transport_generic_free_cmd(se_cmd, 0);

}