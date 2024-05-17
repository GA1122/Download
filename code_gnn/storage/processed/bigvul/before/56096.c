static int perf_event_set_bpf_prog(struct perf_event *event, u32 prog_fd)
{
	struct bpf_prog *prog;

	if (event->attr.type != PERF_TYPE_TRACEPOINT)
		return -EINVAL;

	if (event->tp_event->prog)
		return -EEXIST;

	if (!(event->tp_event->flags & TRACE_EVENT_FL_UKPROBE))
		 
		return -EINVAL;

	prog = bpf_prog_get(prog_fd);
	if (IS_ERR(prog))
		return PTR_ERR(prog);

	if (prog->type != BPF_PROG_TYPE_KPROBE) {
		 
		bpf_prog_put(prog);
		return -EINVAL;
	}

	event->tp_event->prog = prog;

	return 0;
}
