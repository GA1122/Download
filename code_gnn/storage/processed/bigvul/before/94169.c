static int tcm_loop_write_pending(struct se_cmd *se_cmd)
{
	 
	transport_generic_process_write(se_cmd);
	return 0;
}
