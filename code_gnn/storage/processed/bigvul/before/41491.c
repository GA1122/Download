static void dn_keepalive(struct sock *sk)
{
	struct dn_scp *scp = DN_SK(sk);

	 
	if (skb_queue_empty(&scp->other_xmit_queue))
		dn_nsp_send_link(sk, DN_NOCHANGE, 0);
}
