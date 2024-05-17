static int tcp_v4_inbound_md5_hash(struct sock *sk, struct sk_buff *skb)
{
	 
	__u8 *hash_location = NULL;
	struct tcp_md5sig_key *hash_expected;
	const struct iphdr *iph = ip_hdr(skb);
	struct tcphdr *th = tcp_hdr(skb);
	int genhash;
	unsigned char newhash[16];

	hash_expected = tcp_v4_md5_do_lookup(sk, iph->saddr);
	hash_location = tcp_parse_md5sig_option(th);

	 
	if (!hash_expected && !hash_location)
		return 0;

	if (hash_expected && !hash_location) {
		NET_INC_STATS_BH(sock_net(sk), LINUX_MIB_TCPMD5NOTFOUND);
		return 1;
	}

	if (!hash_expected && hash_location) {
		NET_INC_STATS_BH(sock_net(sk), LINUX_MIB_TCPMD5UNEXPECTED);
		return 1;
	}

	 
	genhash = tcp_v4_md5_hash_skb(newhash,
				      hash_expected,
				      NULL, NULL, skb);

	if (genhash || memcmp(hash_location, newhash, 16) != 0) {
		if (net_ratelimit()) {
			printk(KERN_INFO "MD5 Hash failed for (%pI4, %d)->(%pI4, %d)%s\n",
			       &iph->saddr, ntohs(th->source),
			       &iph->daddr, ntohs(th->dest),
			       genhash ? " tcp_v4_calc_md5_hash failed" : "");
		}
		return 1;
	}
	return 0;
}