struct sk_buff *validate_xmit_skb_list(struct sk_buff *skb, struct net_device *dev)
{
	struct sk_buff *next, *head = NULL, *tail;

	for (; skb != NULL; skb = next) {
		next = skb->next;
		skb->next = NULL;

		 
		skb->prev = skb;

		skb = validate_xmit_skb(skb, dev);
		if (!skb)
			continue;

		if (!head)
			head = skb;
		else
			tail->next = skb;
		 
		tail = skb->prev;
	}
	return head;
}
