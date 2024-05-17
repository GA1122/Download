static int afiucv_hs_rcv(struct sk_buff *skb, struct net_device *dev,
	struct packet_type *pt, struct net_device *orig_dev)
{
	struct sock *sk;
	struct iucv_sock *iucv;
	struct af_iucv_trans_hdr *trans_hdr;
	char nullstring[8];
	int err = 0;

	skb_pull(skb, ETH_HLEN);
	trans_hdr = (struct af_iucv_trans_hdr *)skb->data;
	EBCASC(trans_hdr->destAppName, sizeof(trans_hdr->destAppName));
	EBCASC(trans_hdr->destUserID, sizeof(trans_hdr->destUserID));
	EBCASC(trans_hdr->srcAppName, sizeof(trans_hdr->srcAppName));
	EBCASC(trans_hdr->srcUserID, sizeof(trans_hdr->srcUserID));
	memset(nullstring, 0, sizeof(nullstring));
	iucv = NULL;
	sk = NULL;
	read_lock(&iucv_sk_list.lock);
	sk_for_each(sk, &iucv_sk_list.head) {
		if (trans_hdr->flags == AF_IUCV_FLAG_SYN) {
			if ((!memcmp(&iucv_sk(sk)->src_name,
				     trans_hdr->destAppName, 8)) &&
			    (!memcmp(&iucv_sk(sk)->src_user_id,
				     trans_hdr->destUserID, 8)) &&
			    (!memcmp(&iucv_sk(sk)->dst_name, nullstring, 8)) &&
			    (!memcmp(&iucv_sk(sk)->dst_user_id,
				     nullstring, 8))) {
				iucv = iucv_sk(sk);
				break;
			}
		} else {
			if ((!memcmp(&iucv_sk(sk)->src_name,
				     trans_hdr->destAppName, 8)) &&
			    (!memcmp(&iucv_sk(sk)->src_user_id,
				     trans_hdr->destUserID, 8)) &&
			    (!memcmp(&iucv_sk(sk)->dst_name,
				     trans_hdr->srcAppName, 8)) &&
			    (!memcmp(&iucv_sk(sk)->dst_user_id,
				     trans_hdr->srcUserID, 8))) {
				iucv = iucv_sk(sk);
				break;
			}
		}
	}
	read_unlock(&iucv_sk_list.lock);
	if (!iucv)
		sk = NULL;

	 

	switch (trans_hdr->flags) {
	case AF_IUCV_FLAG_SYN:
		 
		err = afiucv_hs_callback_syn(sk, skb);
		break;
	case (AF_IUCV_FLAG_SYN | AF_IUCV_FLAG_ACK):
		 
		err = afiucv_hs_callback_synack(sk, skb);
		break;
	case (AF_IUCV_FLAG_SYN | AF_IUCV_FLAG_FIN):
		 
		err = afiucv_hs_callback_synfin(sk, skb);
		break;
	case (AF_IUCV_FLAG_FIN):
		 
		err = afiucv_hs_callback_fin(sk, skb);
		break;
	case (AF_IUCV_FLAG_WIN):
		err = afiucv_hs_callback_win(sk, skb);
		if (skb->len == sizeof(struct af_iucv_trans_hdr)) {
			kfree_skb(skb);
			break;
		}
		 
	case (AF_IUCV_FLAG_SHT):
		 
		 
	case 0:
		 
		IUCV_SKB_CB(skb)->class = trans_hdr->iucv_hdr.class;
		err = afiucv_hs_callback_rx(sk, skb);
		break;
	default:
		;
	}

	return err;
}
