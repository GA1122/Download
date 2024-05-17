static int reg_event_syscall_enter(struct ftrace_event_file *file,
				   struct ftrace_event_call *call)
{
	struct trace_array *tr = file->tr;
	int ret = 0;
	int num;

	num = ((struct syscall_metadata *)call->data)->syscall_nr;
	if (WARN_ON_ONCE(num < 0 || num >= NR_syscalls))
		return -ENOSYS;
	mutex_lock(&syscall_trace_lock);
	if (!tr->sys_refcount_enter)
		ret = register_trace_sys_enter(ftrace_syscall_enter, tr);
	if (!ret) {
		rcu_assign_pointer(tr->enter_syscall_files[num], file);
		tr->sys_refcount_enter++;
	}
	mutex_unlock(&syscall_trace_lock);
	return ret;
}