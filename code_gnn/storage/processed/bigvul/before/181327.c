 svc_set_num_threads(struct svc_serv *serv, struct svc_pool *pool, int nrservs)
//  
// static int
// svc_start_kthreads(struct svc_serv *serv, struct svc_pool *pool, int nrservs)
  {
  	struct svc_rqst	*rqstp;
  	struct task_struct *task;
  	struct svc_pool *chosen_pool;
	int error = 0;
  	unsigned int state = serv->sv_nrthreads-1;
  	int node;
  
	if (pool == NULL) {
		 
		nrservs -= (serv->sv_nrthreads-1);
	} else {
		spin_lock_bh(&pool->sp_lock);
		nrservs -= pool->sp_nrthreads;
		spin_unlock_bh(&pool->sp_lock);
	}
	 
	while (nrservs > 0) {
// 	do {
  		nrservs--;
  		chosen_pool = choose_pool(serv, pool, &state);
  
  		node = svc_pool_map_get_node(chosen_pool->sp_id);
  		rqstp = svc_prepare_thread(serv, chosen_pool, node);
		if (IS_ERR(rqstp)) {
			error = PTR_ERR(rqstp);
			break;
		}
// 		if (IS_ERR(rqstp))
// 			return PTR_ERR(rqstp);
  
  		__module_get(serv->sv_ops->svo_module);
  		task = kthread_create_on_node(serv->sv_ops->svo_function, rqstp,
  					      node, "%s", serv->sv_name);
  		if (IS_ERR(task)) {
			error = PTR_ERR(task);
  			module_put(serv->sv_ops->svo_module);
  			svc_exit_thread(rqstp);
			break;
// 			return PTR_ERR(task);
  		}
  
  		rqstp->rq_task = task;
 		if (serv->sv_nrpools > 1)
 			svc_pool_map_set_cpumask(task, chosen_pool->sp_id);
  
  		svc_sock_update_bufs(serv);
  		wake_up_process(task);
	}
// 	} while (nrservs > 0);
// 
// 	return 0;
// }
// 
// 
//  
// static int
// svc_signal_kthreads(struct svc_serv *serv, struct svc_pool *pool, int nrservs)
// {
// 	struct task_struct *task;
// 	unsigned int state = serv->sv_nrthreads-1;
// 
  	 
	while (nrservs < 0 &&
	       (task = choose_victim(serv, pool, &state)) != NULL) {
// 	do {
// 		task = choose_victim(serv, pool, &state);
// 		if (task == NULL)
// 			break;
  		send_sig(SIGINT, task, 1);
  		nrservs++;
// 	} while (nrservs < 0);
// 
// 	return 0;
// }
// 
//  
// int
// svc_set_num_threads(struct svc_serv *serv, struct svc_pool *pool, int nrservs)
// {
// 	if (pool == NULL) {
// 		 
// 		nrservs -= (serv->sv_nrthreads-1);
// 	} else {
// 		spin_lock_bh(&pool->sp_lock);
// 		nrservs -= pool->sp_nrthreads;
// 		spin_unlock_bh(&pool->sp_lock);
  	}
  
	return error;
// 	if (nrservs > 0)
// 		return svc_start_kthreads(serv, pool, nrservs);
// 	if (nrservs < 0)
// 		return svc_signal_kthreads(serv, pool, nrservs);
// 	return 0;
  }