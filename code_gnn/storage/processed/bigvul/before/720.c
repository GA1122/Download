int dns_get_ip_from_response(struct dns_response_packet *dns_p,
                             struct dns_options *dns_opts, void *currentip,
                             short currentip_sin_family,
                             void **newip, short *newip_sin_family,
                             void *owner)
{
	struct dns_answer_item *record;
	int family_priority;
	int currentip_found;
	unsigned char *newip4, *newip6;
	int currentip_sel;
	int j;
	int score, max_score;
	int allowed_duplicated_ip;

	family_priority   = dns_opts->family_prio;
	allowed_duplicated_ip = dns_opts->accept_duplicate_ip;
	*newip = newip4   = newip6 = NULL;
	currentip_found   = 0;
	*newip_sin_family = AF_UNSPEC;
	max_score         = -1;

	 

	list_for_each_entry(record, &dns_p->answer_list, list) {
		void *ip;
		unsigned char ip_type;

		if (record->type == DNS_RTYPE_A) {
			ip = &(((struct sockaddr_in *)&record->address)->sin_addr);
			ip_type = AF_INET;
		}
		else if (record->type == DNS_RTYPE_AAAA) {
			ip_type = AF_INET6;
			ip = &(((struct sockaddr_in6 *)&record->address)->sin6_addr);
		}
		else
			continue;
		score = 0;

		 
		if (ip_type == family_priority)
			score += 8;

		 
		for (j = 0; j < dns_opts->pref_net_nb; j++) {

			 
			if (dns_opts->pref_net[j].family != ip_type)
				continue;

			if ((ip_type == AF_INET &&
			     in_net_ipv4(ip,
			                 &dns_opts->pref_net[j].mask.in4,
			                 &dns_opts->pref_net[j].addr.in4)) ||
			    (ip_type == AF_INET6 &&
			     in_net_ipv6(ip,
			                 &dns_opts->pref_net[j].mask.in6,
			                 &dns_opts->pref_net[j].addr.in6))) {
				score += 4;
				break;
			}
		}

		 
		if (owner && snr_check_ip_callback(owner, ip, &ip_type)) {
			if (!allowed_duplicated_ip) {
				continue;
			}
		} else {
			score += 2;
		}

		 
		if (ip_type == currentip_sin_family &&
		    ((currentip_sin_family == AF_INET &&
		      !memcmp(ip, currentip, 4)) ||
		     (currentip_sin_family == AF_INET6 &&
		      !memcmp(ip, currentip, 16)))) {
			score++;
			currentip_sel = 1;
		}
		else
			currentip_sel = 0;

		 
		if (score > max_score) {
			if (ip_type == AF_INET)
				newip4 = ip;
			else
				newip6 = ip;
			currentip_found = currentip_sel;
			if (score == 15)
				return DNS_UPD_NO;
			max_score = score;
		}
	}  

	 
	if (!newip4 && !newip6)
		return DNS_UPD_NO_IP_FOUND;

	 
	if (family_priority == AF_INET) {
		if (newip4) {
			*newip = newip4;
			*newip_sin_family = AF_INET;
		}
		else if (newip6) {
			*newip = newip6;
			*newip_sin_family = AF_INET6;
		}
		if (!currentip_found)
			goto not_found;
	}
	 
	else if (family_priority == AF_INET6) {
		if (newip6) {
			*newip = newip6;
			*newip_sin_family = AF_INET6;
		}
		else if (newip4) {
			*newip = newip4;
			*newip_sin_family = AF_INET;
		}
		if (!currentip_found)
			goto not_found;
	}
	 
	else if (family_priority == AF_UNSPEC) {
		if (newip6) {
			*newip = newip6;
			*newip_sin_family = AF_INET6;
		}
		else if (newip4) {
			*newip = newip4;
			*newip_sin_family = AF_INET;
		}
		if (!currentip_found)
			goto not_found;
	}

	 
	return DNS_UPD_NO;

 not_found:
	list_for_each_entry(record, &dns_p->answer_list, list) {
		 
		LIST_DEL(&record->list);
		LIST_ADDQ(&dns_p->answer_list, &record->list);
		break;
	}
	return DNS_UPD_SRVIP_NOT_FOUND;
}
