static __be16 atalk_checksum(const struct sk_buff *skb, int len)
{
	unsigned long sum;

	 
	sum = atalk_sum_skb(skb, 4, len-4, 0);

	 
	return sum ? htons((unsigned short)sum) : htons(0xFFFF);
}
