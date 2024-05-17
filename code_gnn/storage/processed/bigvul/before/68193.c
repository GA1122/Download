int llc_conn_remove_acked_pdus(struct sock *sk, u8 nr, u16 *how_many_unacked)
{
	int pdu_pos, i;
	struct sk_buff *skb;
	struct llc_pdu_sn *pdu;
	int nbr_acked = 0;
	struct llc_sock *llc = llc_sk(sk);
	int q_len = skb_queue_len(&llc->pdu_unack_q);

	if (!q_len)
		goto out;
	skb = skb_peek(&llc->pdu_unack_q);
	pdu = llc_pdu_sn_hdr(skb);

	 
	pdu_pos = ((int)LLC_2_SEQ_NBR_MODULO + (int)nr -
			(int)LLC_I_GET_NS(pdu)) % LLC_2_SEQ_NBR_MODULO;

	for (i = 0; i < pdu_pos && i < q_len; i++) {
		skb = skb_dequeue(&llc->pdu_unack_q);
		kfree_skb(skb);
		nbr_acked++;
	}
out:
	*how_many_unacked = skb_queue_len(&llc->pdu_unack_q);
	return nbr_acked;
}