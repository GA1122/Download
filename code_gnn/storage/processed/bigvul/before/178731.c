 static struct ip_options *ip_options_get_alloc(const int optlen)
// static struct ip_options_rcu *ip_options_get_alloc(const int optlen)
  {
	return kzalloc(sizeof(struct ip_options) + ((optlen + 3) & ~3),
// 	return kzalloc(sizeof(struct ip_options_rcu) + ((optlen + 3) & ~3),
  		       GFP_KERNEL);
  }