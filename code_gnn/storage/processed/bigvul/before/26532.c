static void pmcraid_unregister_hcams(struct pmcraid_cmd *cmd)
{
	struct pmcraid_instance *pinstance = cmd->drv_inst;

	 
	atomic_set(&pinstance->ccn.ignore, 1);
	atomic_set(&pinstance->ldn.ignore, 1);

	 
	if ((pinstance->force_ioa_reset && !pinstance->ioa_bringdown) ||
	     pinstance->ioa_unit_check) {
		pinstance->force_ioa_reset = 0;
		pinstance->ioa_unit_check = 0;
		pinstance->ioa_state = IOA_STATE_IN_RESET_ALERT;
		pmcraid_reset_alert(cmd);
		return;
	}

	 
	pmcraid_cancel_ldn(cmd);
}
