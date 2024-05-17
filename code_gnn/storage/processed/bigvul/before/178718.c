 static int cipso_v4_delopt(struct ip_options **opt_ptr)
// static int cipso_v4_delopt(struct ip_options_rcu **opt_ptr)
  {
  	int hdr_delta = 0;
	struct ip_options *opt = *opt_ptr;
// 	struct ip_options_rcu *opt = *opt_ptr;
  
	if (opt->srr || opt->rr || opt->ts || opt->router_alert) {
// 	if (opt->opt.srr || opt->opt.rr || opt->opt.ts || opt->opt.router_alert) {
  		u8 cipso_len;
  		u8 cipso_off;
  		unsigned char *cipso_ptr;
  		int iter;
  		int optlen_new;
  
		cipso_off = opt->cipso - sizeof(struct iphdr);
		cipso_ptr = &opt->__data[cipso_off];
// 		cipso_off = opt->opt.cipso - sizeof(struct iphdr);
// 		cipso_ptr = &opt->opt.__data[cipso_off];
  		cipso_len = cipso_ptr[1];
  
		if (opt->srr > opt->cipso)
			opt->srr -= cipso_len;
		if (opt->rr > opt->cipso)
			opt->rr -= cipso_len;
		if (opt->ts > opt->cipso)
			opt->ts -= cipso_len;
		if (opt->router_alert > opt->cipso)
			opt->router_alert -= cipso_len;
		opt->cipso = 0;
// 		if (opt->opt.srr > opt->opt.cipso)
// 			opt->opt.srr -= cipso_len;
// 		if (opt->opt.rr > opt->opt.cipso)
// 			opt->opt.rr -= cipso_len;
// 		if (opt->opt.ts > opt->opt.cipso)
// 			opt->opt.ts -= cipso_len;
// 		if (opt->opt.router_alert > opt->opt.cipso)
// 			opt->opt.router_alert -= cipso_len;
// 		opt->opt.cipso = 0;
  
  		memmove(cipso_ptr, cipso_ptr + cipso_len,
			opt->optlen - cipso_off - cipso_len);
// 			opt->opt.optlen - cipso_off - cipso_len);
  
  		 
  		iter = 0;
  		optlen_new = 0;
		while (iter < opt->optlen)
			if (opt->__data[iter] != IPOPT_NOP) {
				iter += opt->__data[iter + 1];
// 		while (iter < opt->opt.optlen)
// 			if (opt->opt.__data[iter] != IPOPT_NOP) {
// 				iter += opt->opt.__data[iter + 1];
  				optlen_new = iter;
  			} else
  				iter++;
		hdr_delta = opt->optlen;
		opt->optlen = (optlen_new + 3) & ~3;
		hdr_delta -= opt->optlen;
// 		hdr_delta = opt->opt.optlen;
// 		opt->opt.optlen = (optlen_new + 3) & ~3;
// 		hdr_delta -= opt->opt.optlen;
  	} else {
  		 
  		*opt_ptr = NULL;
		hdr_delta = opt->optlen;
		kfree(opt);
// 		hdr_delta = opt->opt.optlen;
// 		call_rcu(&opt->rcu, opt_kfree_rcu);
  	}
  
  	return hdr_delta;
 }