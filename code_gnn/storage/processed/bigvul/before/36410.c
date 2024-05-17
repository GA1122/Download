static int pppol2tp_recv_payload_hook(struct sk_buff *skb)
{
	 
	if (!pskb_may_pull(skb, 2))
		return 1;

	if ((skb->data[0] == 0xff) && (skb->data[1] == 0x03))
		skb_pull(skb, 2);

	return 0;
}
