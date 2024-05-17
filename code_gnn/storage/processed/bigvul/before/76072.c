vrrp_create_sockpool(list l)
{
	vrrp_t *vrrp;
	element e;
	ifindex_t ifindex;
	int proto;
	bool unicast;
	sock_t *sock;

	LIST_FOREACH(vrrp_data->vrrp, vrrp, e) {
		ifindex =
#ifdef _HAVE_VRRP_VMAC_
			  (__test_bit(VRRP_VMAC_XMITBASE_BIT, &vrrp->vmac_flags)) ? IF_BASE_INDEX(vrrp->ifp) :
#endif
										    IF_INDEX(vrrp->ifp);
		unicast = !LIST_ISEMPTY(vrrp->unicast_peer);
#if defined _WITH_VRRP_AUTH_
		if (vrrp->auth_type == VRRP_AUTH_AH)
			proto = IPPROTO_AH;
		else
#endif
			proto = IPPROTO_VRRP;

		 
		if (!(sock = already_exist_sock(l, vrrp->family, proto, ifindex, unicast)))
			sock = alloc_sock(vrrp->family, l, proto, ifindex, unicast);

		 
		rb_insert_sort(&sock->rb_vrid, vrrp, rb_vrid, vrrp_vrid_cmp);

		if (vrrp->kernel_rx_buf_size)
			sock->rx_buf_size += vrrp->kernel_rx_buf_size;
		else if (global_data->vrrp_rx_bufs_policy & RX_BUFS_SIZE)
			sock->rx_buf_size += global_data->vrrp_rx_bufs_size;
		else if (global_data->vrrp_rx_bufs_policy & RX_BUFS_POLICY_ADVERT)
			sock->rx_buf_size += global_data->vrrp_rx_bufs_multiples * vrrp_adv_len(vrrp);
		else if (global_data->vrrp_rx_bufs_policy & RX_BUFS_POLICY_MTU)
			sock->rx_buf_size += global_data->vrrp_rx_bufs_multiples * vrrp->ifp->mtu;
	}
}