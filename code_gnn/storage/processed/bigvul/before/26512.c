static void pmcraid_send_cmd(
	struct pmcraid_cmd *cmd,
	void (*cmd_done) (struct pmcraid_cmd *),
	unsigned long timeout,
	void (*timeout_func) (struct pmcraid_cmd *)
)
{
	 
	cmd->cmd_done = cmd_done;

	if (timeout_func) {
		 
		cmd->timer.data = (unsigned long)cmd;
		cmd->timer.expires = jiffies + timeout;
		cmd->timer.function = (void (*)(unsigned long))timeout_func;
		add_timer(&cmd->timer);
	}

	 
	_pmcraid_fire_command(cmd);
}