static inline void update_cgrp_time_from_cpuctx(struct perf_cpu_context *cpuctx)
{
	struct perf_cgroup *cgrp_out = cpuctx->cgrp;
	if (cgrp_out)
		__update_cgrp_time(cgrp_out);
}
