static inline int may_ptrace_stop(void)
{
	if (!likely(task_ptrace(current)))
		return 0;
	 
	if (unlikely(current->mm->core_state) &&
	    unlikely(current->mm == current->parent->mm))
		return 0;

	return 1;
}
