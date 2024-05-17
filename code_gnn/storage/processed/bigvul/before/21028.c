static void kmem_cgroup_destroy(struct cgroup_subsys *ss,
				struct cgroup *cont)
{
	mem_cgroup_sockets_destroy(cont, ss);
}
