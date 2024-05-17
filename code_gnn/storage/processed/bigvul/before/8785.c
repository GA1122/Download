static NOINLINE char *xmalloc_optname_optval(uint8_t *option, const struct dhcp_optflag *optflag, const char *opt_name)
{
	unsigned upper_length;
	int len, type, optlen;
	char *dest, *ret;

	 
	len = option[-OPT_DATA + OPT_LEN];

	type = optflag->flags & OPTION_TYPE_MASK;
	optlen = dhcp_option_lengths[type];
	upper_length = len_of_option_as_string[type]
		* ((unsigned)(len + optlen) / (unsigned)optlen);

	dest = ret = xmalloc(upper_length + strlen(opt_name) + 2);
	dest += sprintf(ret, "%s=", opt_name);

	while (len >= optlen) {
		switch (type) {
		case OPTION_IP:
		case OPTION_IP_PAIR:
			dest += sprint_nip(dest, "", option);
			if (type == OPTION_IP)
				break;
			dest += sprint_nip(dest, "/", option + 4);
			break;
		case OPTION_U8:
			dest += sprintf(dest, "%u", *option);
			break;
		case OPTION_U16: {
			uint16_t val_u16;
			move_from_unaligned16(val_u16, option);
			dest += sprintf(dest, "%u", ntohs(val_u16));
			break;
		}
		case OPTION_S32:
		case OPTION_U32: {
			uint32_t val_u32;
			move_from_unaligned32(val_u32, option);
			dest += sprintf(dest, type == OPTION_U32 ? "%lu" : "%ld", (unsigned long) ntohl(val_u32));
			break;
		}
		 
		case OPTION_STRING:
		case OPTION_STRING_HOST:
			memcpy(dest, option, len);
			dest[len] = '\0';
			if (type == OPTION_STRING_HOST && !good_hostname(dest))
				safe_strncpy(dest, "bad", len);
			return ret;
		case OPTION_STATIC_ROUTES: {
			 
			const char *pfx = "";

			while (len >= 1 + 4) {  
				uint32_t nip;
				uint8_t *p;
				unsigned mask;
				int bytes;

				mask = *option++;
				if (mask > 32)
					break;
				len--;

				nip = 0;
				p = (void*) &nip;
				bytes = (mask + 7) / 8;  
				while (--bytes >= 0) {
					*p++ = *option++;
					len--;
				}
				if (len < 4)
					break;

				 
				dest += sprint_nip(dest, pfx, (void*) &nip);
				pfx = " ";
				dest += sprintf(dest, "/%u ", mask);
				 
				dest += sprint_nip(dest, "", option);
				option += 4;
				len -= 4;
			}

			return ret;
		}
		case OPTION_6RD:
			 
			if (len >= (1 + 1 + 16 + 4)
			 && option[0] <= 32
			 && (option[1] + 32 - option[0]) <= 128
			) {
				 
				dest += sprintf(dest, "%u ", *option++);
				 
				dest += sprintf(dest, "%u ", *option++);
				 
				dest += sprint_nip6(dest,   option);
				option += 16;
				len -= 1 + 1 + 16 + 4;
				 
				while (1) {
					 
					dest += sprint_nip(dest, " ", option);
					option += 4;
					len -= 4;  
					if (len < 0)
						break;  
				}
			}

			return ret;
#if ENABLE_FEATURE_UDHCP_RFC3397
		case OPTION_DNS_STRING:
			 
			dest = dname_dec(option, len, ret);
			if (dest) {
				free(ret);
				return dest;
			}
			 
			return ret;
		case OPTION_SIP_SERVERS:
			 
			option++;
			len--;
			if (option[-1] == 0) {
				dest = dname_dec(option, len, ret);
				if (dest) {
					free(ret);
					return dest;
				}
			} else
			if (option[-1] == 1) {
				const char *pfx = "";
				while (1) {
					len -= 4;
					if (len < 0)
						break;
					dest += sprint_nip(dest, pfx, option);
					pfx = " ";
					option += 4;
				}
			}
			return ret;
#endif
		}  

		 
		option += optlen;
		len -= optlen;
		if (len < optlen  )
			break;
		*dest++ = ' ';
		*dest = '\0';
	}  

	return ret;
}
