inherit_task_group(struct perf_event *event, struct task_struct *parent,
		   struct perf_event_context *parent_ctx,
		   struct task_struct *child, int ctxn,
		   int *inherited_all)
{
	int ret;
	struct perf_event_context *child_ctx;

	if (!event->attr.inherit) {
		*inherited_all = 0;
		return 0;
	}

	child_ctx = child->perf_event_ctxp[ctxn];
	if (!child_ctx) {
		 

		child_ctx = alloc_perf_context(event->pmu, child);
		if (!child_ctx)
			return -ENOMEM;

		child->perf_event_ctxp[ctxn] = child_ctx;
	}

	ret = inherit_group(event, parent, parent_ctx,
			    child, child_ctx);

	if (ret)
		*inherited_all = 0;

	return ret;
}