static void sctp_control_set_owner_w(struct sctp_chunk *chunk)
{
	struct sctp_association *asoc = chunk->asoc;
	struct sk_buff *skb = chunk->skb;

	 
	skb->sk = asoc ? asoc->base.sk : NULL;
	skb->destructor = sctp_control_release_owner;
}
