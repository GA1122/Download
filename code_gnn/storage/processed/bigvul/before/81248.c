static void add_tracer_options(struct trace_array *tr, struct tracer *t)
{
	 
	if (!tr->dir)
		return;

	create_trace_option_files(tr, t);
}
