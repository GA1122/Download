 static inline bool unconditional(const struct ip6t_ip6 *ipv6)
// static inline bool unconditional(const struct ip6t_entry *e)
  {
  	static const struct ip6t_ip6 uncond;
  
	return memcmp(ipv6, &uncond, sizeof(uncond)) == 0;
// 	return e->target_offset == sizeof(struct ip6t_entry) &&
// 	       memcmp(&e->ipv6, &uncond, sizeof(uncond)) == 0;
  }