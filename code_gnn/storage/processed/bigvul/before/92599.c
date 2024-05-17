static inline void list_add_leaf_cfs_rq(struct cfs_rq *cfs_rq)
{
	if (!cfs_rq->on_list) {
		struct rq *rq = rq_of(cfs_rq);
		int cpu = cpu_of(rq);
		 
		if (cfs_rq->tg->parent &&
		    cfs_rq->tg->parent->cfs_rq[cpu]->on_list) {
			 
			list_add_tail_rcu(&cfs_rq->leaf_cfs_rq_list,
				&(cfs_rq->tg->parent->cfs_rq[cpu]->leaf_cfs_rq_list));
			 
			rq->tmp_alone_branch = &rq->leaf_cfs_rq_list;
		} else if (!cfs_rq->tg->parent) {
			 
			list_add_tail_rcu(&cfs_rq->leaf_cfs_rq_list,
				&rq->leaf_cfs_rq_list);
			 
			rq->tmp_alone_branch = &rq->leaf_cfs_rq_list;
		} else {
			 
			list_add_rcu(&cfs_rq->leaf_cfs_rq_list,
				rq->tmp_alone_branch);
			 
			rq->tmp_alone_branch = &cfs_rq->leaf_cfs_rq_list;
		}

		cfs_rq->on_list = 1;
	}
}
