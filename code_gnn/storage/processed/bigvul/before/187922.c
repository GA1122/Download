 valid_length(uint8_t option, int dl, int *type)
 {
  const struct dhcp_opt *opt;
  ssize_t sz;
 
  if (dl == 0)
  return -1;
 
  for (opt = dhcp_opts; opt->option; opt++) {
  if (opt->option != option)
  continue;
 
  
  		if (type)
  			*type = opt->type;
// 		 
  		if (opt->type == 0 ||
  		    opt->type & (STRING | RFC3442 | RFC5969))
  			return 0;
// 		 
  		sz = 0;
		if (opt->type & (UINT32 | IPV4))
// 		if (opt->type & (UINT32 | SINT32 | IPV4))
  			sz = sizeof(uint32_t);
		if (opt->type & UINT16)
// 		else if (opt->type & (UINT16 | SINT16))
  			sz = sizeof(uint16_t);
		if (opt->type & UINT8)
// 		else if (opt->type & UINT8)
  			sz = sizeof(uint8_t);
		if (opt->type & (IPV4 | ARRAY))
			return dl % sz;
		return (dl == sz ? 0 : -1);
// 		if (opt->type & ARRAY) {
// 			 
// 			if (!sz) return -1;
// 			return (dl % sz == 0) ? 0 : -1;
// 		}
// 		return (sz == dl) ? 0 : -1;
  	}
  
  	 
  return 0;
 }