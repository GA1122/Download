static int vmci_transport_recv_dgram_cb(void *data, struct vmci_datagram *dg)
{
	struct sock *sk;
	size_t size;
	struct sk_buff *skb;
	struct vsock_sock *vsk;

	sk = (struct sock *)data;

	 
	vsk = vsock_sk(sk);
	if (!vmci_transport_allow_dgram(vsk, dg->src.context))
		return VMCI_ERROR_NO_ACCESS;

	size = VMCI_DG_SIZE(dg);

	 
	skb = alloc_skb(size, GFP_ATOMIC);
	if (skb) {
		 
		sock_hold(sk);
		skb_put(skb, size);
		memcpy(skb->data, dg, size);
		sk_receive_skb(sk, skb, 0);
	}

	return VMCI_SUCCESS;
}
