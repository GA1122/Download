svc_pool_map_get(void)
{
	struct svc_pool_map *m = &svc_pool_map;
	int npools = -1;

	mutex_lock(&svc_pool_map_mutex);

	if (m->count++) {
		mutex_unlock(&svc_pool_map_mutex);
		return m->npools;
	}

	if (m->mode == SVC_POOL_AUTO)
		m->mode = svc_pool_map_choose_mode();

	switch (m->mode) {
	case SVC_POOL_PERCPU:
		npools = svc_pool_map_init_percpu(m);
		break;
	case SVC_POOL_PERNODE:
		npools = svc_pool_map_init_pernode(m);
		break;
	}

	if (npools < 0) {
		 
		npools = 1;
		m->mode = SVC_POOL_GLOBAL;
	}
	m->npools = npools;

	mutex_unlock(&svc_pool_map_mutex);
	return m->npools;
}