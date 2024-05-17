void tipc_sk_rht_destroy(struct net *net)
{
	struct tipc_net *tn = net_generic(net, tipc_net_id);

	 
	synchronize_net();

	rhashtable_destroy(&tn->sk_rht);
}
