static void pppol2tp_next_tunnel(struct net *net, struct pppol2tp_seq_data *pd)
{
	for (;;) {
		pd->tunnel = l2tp_tunnel_find_nth(net, pd->tunnel_idx);
		pd->tunnel_idx++;

		if (pd->tunnel == NULL)
			break;

		 
		if (pd->tunnel->version < 3)
			break;
	}
}