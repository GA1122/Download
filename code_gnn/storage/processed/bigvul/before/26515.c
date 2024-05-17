static void pmcraid_set_supported_devs(struct pmcraid_cmd *cmd)
{
	struct pmcraid_ioarcb *ioarcb = &cmd->ioa_cb->ioarcb;
	void (*cmd_done) (struct pmcraid_cmd *) = pmcraid_complete_ioa_reset;

	pmcraid_reinit_cmdblk(cmd);

	ioarcb->resource_handle = cpu_to_le32(PMCRAID_IOA_RES_HANDLE);
	ioarcb->request_type = REQ_TYPE_IOACMD;
	ioarcb->cdb[0] = PMCRAID_SET_SUPPORTED_DEVICES;
	ioarcb->cdb[1] = ALL_DEVICES_SUPPORTED;

	 
	if (cmd->drv_inst->reinit_cfg_table) {
		cmd->drv_inst->reinit_cfg_table = 0;
		cmd->release = 1;
		cmd_done = pmcraid_reinit_cfgtable_done;
	}

	 
	pmcraid_send_cmd(cmd,
			 cmd_done,
			 PMCRAID_SET_SUP_DEV_TIMEOUT,
			 pmcraid_timeout_handler);
	return;
}
