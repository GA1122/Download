void rds_loop_exit(void)
{
	struct rds_loop_connection *lc, *_lc;
	LIST_HEAD(tmp_list);

	 
	spin_lock_irq(&loop_conns_lock);
	list_splice(&loop_conns, &tmp_list);
	INIT_LIST_HEAD(&loop_conns);
	spin_unlock_irq(&loop_conns_lock);

	list_for_each_entry_safe(lc, _lc, &tmp_list, loop_node) {
		WARN_ON(lc->conn->c_passive);
		rds_conn_destroy(lc->conn);
	}
}
