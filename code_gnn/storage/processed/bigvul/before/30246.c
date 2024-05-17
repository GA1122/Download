static int function_stat_show(struct seq_file *m, void *v)
{
	struct ftrace_profile *rec = v;
	char str[KSYM_SYMBOL_LEN];
	int ret = 0;
#ifdef CONFIG_FUNCTION_GRAPH_TRACER
	static struct trace_seq s;
	unsigned long long avg;
	unsigned long long stddev;
#endif
	mutex_lock(&ftrace_profile_lock);

	 
	if (unlikely(rec->counter == 0)) {
		ret = -EBUSY;
		goto out;
	}

	kallsyms_lookup(rec->ip, NULL, NULL, NULL, str);
	seq_printf(m, "  %-30.30s  %10lu", str, rec->counter);

#ifdef CONFIG_FUNCTION_GRAPH_TRACER
	seq_printf(m, "    ");
	avg = rec->time;
	do_div(avg, rec->counter);

	 
	if (rec->counter <= 1)
		stddev = 0;
	else {
		stddev = rec->time_squared - rec->counter * avg * avg;
		 
		do_div(stddev, (rec->counter - 1) * 1000);
	}

	trace_seq_init(&s);
	trace_print_graph_duration(rec->time, &s);
	trace_seq_puts(&s, "    ");
	trace_print_graph_duration(avg, &s);
	trace_seq_puts(&s, "    ");
	trace_print_graph_duration(stddev, &s);
	trace_print_seq(m, &s);
#endif
	seq_putc(m, '\n');
out:
	mutex_unlock(&ftrace_profile_lock);

	return ret;
}
