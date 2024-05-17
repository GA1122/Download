static void migrate_task_rq_fair(struct task_struct *p, int new_cpu)
{
	 
	if (p->state == TASK_WAKING) {
		struct sched_entity *se = &p->se;
		struct cfs_rq *cfs_rq = cfs_rq_of(se);
		u64 min_vruntime;

#ifndef CONFIG_64BIT
		u64 min_vruntime_copy;

		do {
			min_vruntime_copy = cfs_rq->min_vruntime_copy;
			smp_rmb();
			min_vruntime = cfs_rq->min_vruntime;
		} while (min_vruntime != min_vruntime_copy);
#else
		min_vruntime = cfs_rq->min_vruntime;
#endif

		se->vruntime -= min_vruntime;
	}

	if (p->on_rq == TASK_ON_RQ_MIGRATING) {
		 
		lockdep_assert_held(&task_rq(p)->lock);
		detach_entity_cfs_rq(&p->se);

	} else {
		 
		remove_entity_load_avg(&p->se);
	}

	 
	p->se.avg.last_update_time = 0;

	 
	p->se.exec_start = 0;

	update_scan_period(p, new_cpu);
}
