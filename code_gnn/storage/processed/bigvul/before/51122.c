void __audit_free(struct task_struct *tsk)
{
	struct audit_context *context;

	context = audit_take_context(tsk, 0, 0);
	if (!context)
		return;

	 
	 
	if (context->in_syscall && context->current_state == AUDIT_RECORD_CONTEXT)
		audit_log_exit(context, tsk);
	if (!list_empty(&context->killed_trees))
		audit_kill_trees(&context->killed_trees);

	audit_free_context(context);
}
