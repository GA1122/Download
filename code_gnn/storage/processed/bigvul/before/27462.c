 static struct xfrm6_tunnel_spi *__xfrm6_tunnel_spi_lookup(struct net *net, xfrm_address_t *saddr)
 {
 	struct xfrm6_tunnel_net *xfrm6_tn = xfrm6_tunnel_pernet(net);
	struct xfrm6_tunnel_spi *x6spi;
	struct hlist_node *pos;

	hlist_for_each_entry_rcu(x6spi, pos,
			     &xfrm6_tn->spi_byaddr[xfrm6_tunnel_spi_hash_byaddr(saddr)],
			     list_byaddr) {
		if (memcmp(&x6spi->addr, saddr, sizeof(x6spi->addr)) == 0)
			return x6spi;
	}

	return NULL;
}