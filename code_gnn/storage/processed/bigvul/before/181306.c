 lockd(void *vrqstp)
  {
  	int		err = 0;
  	struct svc_rqst *rqstp = vrqstp;
// 	struct net *net = &init_net;
// 	struct lockd_net *ln = net_generic(net, lockd_net_id);
  
  	 
  	set_freezable();
 
 	 
 	allow_signal(SIGKILL);
 
 	dprintk("NFS locking service started (ver " LOCKD_VERSION ").\n");
 
 	 
 	while (!kthread_should_stop()) {
 		long timeout = MAX_SCHEDULE_TIMEOUT;
 		RPC_IFDEBUG(char buf[RPC_MAX_ADDRBUFLEN]);
 
 		 
 		rqstp->rq_server->sv_maxconn = nlm_max_connections;
 
 		if (signalled()) {
 			flush_signals(current);
 			restart_grace();
 			continue;
 		}
 
 		timeout = nlmsvc_retry_blocked();
 
 		 
 		err = svc_recv(rqstp, timeout);
 		if (err == -EAGAIN || err == -EINTR)
 			continue;
 		dprintk("lockd: request from %s\n",
 				svc_print_addr(rqstp, buf, sizeof(buf)));
 
 		svc_process(rqstp);
 	}
 	flush_signals(current);
  	if (nlmsvc_ops)
  		nlmsvc_invalidate_all();
  	nlm_shutdown_hosts();
// 	cancel_delayed_work_sync(&ln->grace_period_end);
// 	locks_end_grace(&ln->lockd_manager);
  	return 0;
  }