svc_destroy(struct svc_serv *serv)
{
	dprintk("svc: svc_destroy(%s, %d)\n",
				serv->sv_program->pg_name,
				serv->sv_nrthreads);

	if (serv->sv_nrthreads) {
		if (--(serv->sv_nrthreads) != 0) {
			svc_sock_update_bufs(serv);
			return;
		}
	} else
		printk("svc_destroy: no threads for serv=%p!\n", serv);

	del_timer_sync(&serv->sv_temptimer);

	 
	BUG_ON(!list_empty(&serv->sv_permsocks));
	BUG_ON(!list_empty(&serv->sv_tempsocks));

	cache_clean_deferred(serv);

	if (svc_serv_is_pooled(serv))
		svc_pool_map_put();

	kfree(serv->sv_pools);
	kfree(serv);
}