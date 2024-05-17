  struct sctp_chunk *sctp_process_asconf(struct sctp_association *asoc,
  				       struct sctp_chunk *asconf)
  {
// 	sctp_addip_chunk_t *addip = (sctp_addip_chunk_t *) asconf->chunk_hdr;
// 	bool all_param_pass = true;
// 	union sctp_params param;
  	sctp_addiphdr_t		*hdr;
  	union sctp_addr_param	*addr_param;
  	sctp_addip_param_t	*asconf_param;
  	struct sctp_chunk	*asconf_ack;
  	__be16	err_code;
  	int	length = 0;
  	int	chunk_len;
  	__u32	serial;
	int	all_param_pass = 1;
  
  	chunk_len = ntohs(asconf->chunk_hdr->length) - sizeof(sctp_chunkhdr_t);
  	hdr = (sctp_addiphdr_t *)asconf->skb->data;
 	serial = ntohl(hdr->serial);
 
 	 
 	length = sizeof(sctp_addiphdr_t);
 	addr_param = (union sctp_addr_param *)(asconf->skb->data + length);
 	chunk_len -= length;
 
 	 
 	length = ntohs(addr_param->p.length);
 	asconf_param = (void *)addr_param + length;
 	chunk_len -= length;
 
 	 
 	asconf_ack = sctp_make_asconf_ack(asoc, serial, chunk_len * 4);
 	if (!asconf_ack)
  		goto done;
  
  	 
	while (chunk_len > 0) {
// 	sctp_walk_params(param, addip, addip_hdr.params) {
// 		 
// 		if (param.p->type == SCTP_PARAM_IPV4_ADDRESS ||
// 		    param.p->type == SCTP_PARAM_IPV6_ADDRESS)
// 			continue;
// 
  		err_code = sctp_process_asconf_param(asoc, asconf,
						     asconf_param);
// 						     param.addip);
  		 
		if (SCTP_ERROR_NO_ERROR != err_code)
			all_param_pass = 0;
// 		if (err_code != SCTP_ERROR_NO_ERROR)
// 			all_param_pass = false;
  		if (!all_param_pass)
			sctp_add_asconf_response(asconf_ack,
						 asconf_param->crr_id, err_code,
						 asconf_param);
// 			sctp_add_asconf_response(asconf_ack, param.addip->crr_id,
// 						 err_code, param.addip);
  
  		 
		if (SCTP_ERROR_RSRC_LOW == err_code)
// 		if (err_code == SCTP_ERROR_RSRC_LOW)
  			goto done;
		 
		length = ntohs(asconf_param->param_hdr.length);
		asconf_param = (void *)asconf_param + length;
		chunk_len -= length;
  	}
  done:
  	asoc->peer.addip_serial++;
  
 	 
 	if (asconf_ack) {
 		sctp_chunk_hold(asconf_ack);
 		list_add_tail(&asconf_ack->transmitted_list,
 			      &asoc->asconf_ack_list);
 	}
 
 	return asconf_ack;
 }