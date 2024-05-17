void tracing_record_taskinfo_sched_switch(struct task_struct *prev,
					  struct task_struct *next, int flags)
{
	bool done;

	if (tracing_record_taskinfo_skip(flags))
		return;

	 
	done  = !(flags & TRACE_RECORD_CMDLINE) || trace_save_cmdline(prev);
	done &= !(flags & TRACE_RECORD_CMDLINE) || trace_save_cmdline(next);
	done &= !(flags & TRACE_RECORD_TGID) || trace_save_tgid(prev);
	done &= !(flags & TRACE_RECORD_TGID) || trace_save_tgid(next);

	 
	if (!done)
		return;

	__this_cpu_write(trace_taskinfo_save, false);
}