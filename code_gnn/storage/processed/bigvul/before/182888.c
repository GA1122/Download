  void __ip_select_ident(struct net *net, struct iphdr *iph, int segs)
  {
	static u32 ip_idents_hashrnd __read_mostly;
  	u32 hash, id;
  
	net_get_random_once(&ip_idents_hashrnd, sizeof(ip_idents_hashrnd));
// 	 
// 	if (unlikely(siphash_key_is_zero(&net->ipv4.ip_id_key)))
// 		get_random_bytes(&net->ipv4.ip_id_key,
// 				 sizeof(net->ipv4.ip_id_key));
  
	hash = jhash_3words((__force u32)iph->daddr,
// 	hash = siphash_3u32((__force u32)iph->daddr,
  			    (__force u32)iph->saddr,
			    iph->protocol ^ net_hash_mix(net),
			    ip_idents_hashrnd);
// 			    iph->protocol,
// 			    &net->ipv4.ip_id_key);
  	id = ip_idents_reserve(hash, segs);
  	iph->id = htons(id);
  }