static int __mem_cgroup_try_charge(struct mm_struct *mm,
				   gfp_t gfp_mask,
				   unsigned int nr_pages,
				   struct mem_cgroup **ptr,
				   bool oom)
{
	unsigned int batch = max(CHARGE_BATCH, nr_pages);
	int nr_oom_retries = MEM_CGROUP_RECLAIM_RETRIES;
	struct mem_cgroup *memcg = NULL;
	int ret;

	 
	if (unlikely(test_thread_flag(TIF_MEMDIE)
		     || fatal_signal_pending(current)))
		goto bypass;

	 
	if (!*ptr && !mm)
		*ptr = root_mem_cgroup;
again:
	if (*ptr) {  
		memcg = *ptr;
		VM_BUG_ON(css_is_removed(&memcg->css));
		if (mem_cgroup_is_root(memcg))
			goto done;
		if (nr_pages == 1 && consume_stock(memcg))
			goto done;
		css_get(&memcg->css);
	} else {
		struct task_struct *p;

		rcu_read_lock();
		p = rcu_dereference(mm->owner);
		 
		memcg = mem_cgroup_from_task(p);
		if (!memcg)
			memcg = root_mem_cgroup;
		if (mem_cgroup_is_root(memcg)) {
			rcu_read_unlock();
			goto done;
		}
		if (nr_pages == 1 && consume_stock(memcg)) {
			 
			rcu_read_unlock();
			goto done;
		}
		 
		if (!css_tryget(&memcg->css)) {
			rcu_read_unlock();
			goto again;
		}
		rcu_read_unlock();
	}

	do {
		bool oom_check;

		 
		if (fatal_signal_pending(current)) {
			css_put(&memcg->css);
			goto bypass;
		}

		oom_check = false;
		if (oom && !nr_oom_retries) {
			oom_check = true;
			nr_oom_retries = MEM_CGROUP_RECLAIM_RETRIES;
		}

		ret = mem_cgroup_do_charge(memcg, gfp_mask, batch, oom_check);
		switch (ret) {
		case CHARGE_OK:
			break;
		case CHARGE_RETRY:  
			batch = nr_pages;
			css_put(&memcg->css);
			memcg = NULL;
			goto again;
		case CHARGE_WOULDBLOCK:  
			css_put(&memcg->css);
			goto nomem;
		case CHARGE_NOMEM:  
			if (!oom) {
				css_put(&memcg->css);
				goto nomem;
			}
			 
			nr_oom_retries--;
			break;
		case CHARGE_OOM_DIE:  
			css_put(&memcg->css);
			goto bypass;
		}
	} while (ret != CHARGE_OK);

	if (batch > nr_pages)
		refill_stock(memcg, batch - nr_pages);
	css_put(&memcg->css);
done:
	*ptr = memcg;
	return 0;
nomem:
	*ptr = NULL;
	return -ENOMEM;
bypass:
	*ptr = root_mem_cgroup;
	return -EINTR;
}
