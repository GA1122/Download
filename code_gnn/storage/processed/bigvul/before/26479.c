static void pmcraid_prepare_cancel_cmd(
	struct pmcraid_cmd *cmd,
	struct pmcraid_cmd *cmd_to_cancel
)
{
	struct pmcraid_ioarcb *ioarcb = &cmd->ioa_cb->ioarcb;
	__be64 ioarcb_addr = cmd_to_cancel->ioa_cb->ioarcb.ioarcb_bus_addr;

	 
	ioarcb->resource_handle = cmd_to_cancel->ioa_cb->ioarcb.resource_handle;
	ioarcb->request_type = REQ_TYPE_IOACMD;
	memset(ioarcb->cdb, 0, PMCRAID_MAX_CDB_LEN);
	ioarcb->cdb[0] = PMCRAID_ABORT_CMD;

	 
	ioarcb_addr = cpu_to_be64(ioarcb_addr);
	memcpy(&(ioarcb->cdb[2]), &ioarcb_addr, sizeof(ioarcb_addr));
}