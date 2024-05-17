svc_exit_thread(struct svc_rqst *rqstp)
{
	struct svc_serv	*serv = rqstp->rq_server;
	struct svc_pool	*pool = rqstp->rq_pool;

	spin_lock_bh(&pool->sp_lock);
	pool->sp_nrthreads--;
	if (!test_and_set_bit(RQ_VICTIM, &rqstp->rq_flags))
		list_del_rcu(&rqstp->rq_all);
	spin_unlock_bh(&pool->sp_lock);

	svc_rqst_free(rqstp);

	 
	if (serv)
		svc_destroy(serv);
}