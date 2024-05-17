rpc_init_mempool(void)
{
	 
	rpc_init_wait_queue(&delay_queue, "delayq");
	if (!rpciod_start())
		goto err_nomem;

	rpc_task_slabp = kmem_cache_create("rpc_tasks",
					     sizeof(struct rpc_task),
					     0, SLAB_HWCACHE_ALIGN,
					     NULL);
	if (!rpc_task_slabp)
		goto err_nomem;
	rpc_buffer_slabp = kmem_cache_create("rpc_buffers",
					     RPC_BUFFER_MAXSIZE,
					     0, SLAB_HWCACHE_ALIGN,
					     NULL);
	if (!rpc_buffer_slabp)
		goto err_nomem;
	rpc_task_mempool = mempool_create_slab_pool(RPC_TASK_POOLSIZE,
						    rpc_task_slabp);
	if (!rpc_task_mempool)
		goto err_nomem;
	rpc_buffer_mempool = mempool_create_slab_pool(RPC_BUFFER_POOLSIZE,
						      rpc_buffer_slabp);
	if (!rpc_buffer_mempool)
		goto err_nomem;
	return 0;
err_nomem:
	rpc_destroy_mempool();
	return -ENOMEM;
}