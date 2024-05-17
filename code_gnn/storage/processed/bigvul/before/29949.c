void udp_proc_unregister(struct net *net, struct udp_seq_afinfo *afinfo)
{
	remove_proc_entry(afinfo->name, net->proc_net);
}
