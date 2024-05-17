static void tcm_loop_check_stop_free(struct se_cmd *se_cmd)
{
	 
	if (se_cmd->se_tmr_req)
		return;
	 
	transport_generic_free_cmd(se_cmd, 0, 0);
}
