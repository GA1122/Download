static void dn_user_copy(struct sk_buff *skb, struct optdata_dn *opt)
{
	unsigned char *ptr = skb->data;
	u16 len = *ptr++;  

	BUG_ON(len > 16);  
	opt->opt_optl   = cpu_to_le16(len);
	opt->opt_status = 0;
	memcpy(opt->opt_data, ptr, len);
	skb_pull(skb, len + 1);
}
