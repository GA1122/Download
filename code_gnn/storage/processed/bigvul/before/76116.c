add_script_param(notify_script_t *script, char *param)
{
	 

	if (script->args[script->num_args + 1]) {
		log_message(LOG_INFO, "notify_fifo_script %s no room to add parameter %s", script->args[0], param);
		return;
	}

	 
	script->args[script->num_args++] = param;
}
