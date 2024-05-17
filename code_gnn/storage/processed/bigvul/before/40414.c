static void __ipxitf_down(struct ipx_interface *intrfc)
{
	struct sock *s;
	struct hlist_node *t;

	 
	ipxrtr_del_routes(intrfc);

	spin_lock_bh(&intrfc->if_sklist_lock);
	 
	sk_for_each_safe(s, t, &intrfc->if_sklist) {
		struct ipx_sock *ipxs = ipx_sk(s);

		s->sk_err = ENOLINK;
		s->sk_error_report(s);
		ipxs->intrfc = NULL;
		ipxs->port   = 0;
		sock_set_flag(s, SOCK_ZAPPED);  
		sk_del_node_init(s);
	}
	INIT_HLIST_HEAD(&intrfc->if_sklist);
	spin_unlock_bh(&intrfc->if_sklist_lock);

	 
	list_del(&intrfc->node);

	 
	if (intrfc == ipx_primary_net)
		ipxitf_clear_primary_net();
	if (intrfc == ipx_internal_net)
		ipx_internal_net = NULL;

	if (intrfc->if_dev)
		dev_put(intrfc->if_dev);
	kfree(intrfc);
}
