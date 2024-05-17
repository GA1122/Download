void megasas_free_cmds(struct megasas_instance *instance)
{
	int i;

	 
	megasas_teardown_frame_pool(instance);

	 
	for (i = 0; i < instance->max_mfi_cmds; i++)

		kfree(instance->cmd_list[i]);

	 
	kfree(instance->cmd_list);
	instance->cmd_list = NULL;

	INIT_LIST_HEAD(&instance->cmd_pool);
}
