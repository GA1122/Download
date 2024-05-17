static inline int perf_cgroup_connect(int fd, struct perf_event *event,
				      struct perf_event_attr *attr,
				      struct perf_event *group_leader)
{
	struct perf_cgroup *cgrp;
	struct cgroup_subsys_state *css;
	struct file *file;
	int ret = 0, fput_needed;

	file = fget_light(fd, &fput_needed);
	if (!file)
		return -EBADF;

	css = cgroup_css_from_dir(file, perf_subsys_id);
	if (IS_ERR(css)) {
		ret = PTR_ERR(css);
		goto out;
	}

	cgrp = container_of(css, struct perf_cgroup, css);
	event->cgrp = cgrp;

	 
	perf_get_cgroup(event);

	 
	if (group_leader && group_leader->cgrp != cgrp) {
		perf_detach_cgroup(event);
		ret = -EINVAL;
	}
out:
	fput_light(file, fput_needed);
	return ret;
}
