int mem_cgroup_swappiness(struct mem_cgroup *memcg)
{
	struct cgroup *cgrp = memcg->css.cgroup;

	 
	if (cgrp->parent == NULL)
		return vm_swappiness;

	return memcg->swappiness;
}