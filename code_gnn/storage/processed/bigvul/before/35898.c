static sctp_ierror_t sctp_verify_param(struct net *net,
					const struct sctp_endpoint *ep,
					const struct sctp_association *asoc,
					union sctp_params param,
					sctp_cid_t cid,
					struct sctp_chunk *chunk,
					struct sctp_chunk **err_chunk)
{
	struct sctp_hmac_algo_param *hmacs;
	int retval = SCTP_IERROR_NO_ERROR;
	__u16 n_elt, id = 0;
	int i;

	 

	switch (param.p->type) {
	case SCTP_PARAM_IPV4_ADDRESS:
	case SCTP_PARAM_IPV6_ADDRESS:
	case SCTP_PARAM_COOKIE_PRESERVATIVE:
	case SCTP_PARAM_SUPPORTED_ADDRESS_TYPES:
	case SCTP_PARAM_STATE_COOKIE:
	case SCTP_PARAM_HEARTBEAT_INFO:
	case SCTP_PARAM_UNRECOGNIZED_PARAMETERS:
	case SCTP_PARAM_ECN_CAPABLE:
	case SCTP_PARAM_ADAPTATION_LAYER_IND:
		break;

	case SCTP_PARAM_SUPPORTED_EXT:
		if (!sctp_verify_ext_param(net, param))
			return SCTP_IERROR_ABORT;
		break;

	case SCTP_PARAM_SET_PRIMARY:
		if (net->sctp.addip_enable)
			break;
		goto fallthrough;

	case SCTP_PARAM_HOST_NAME_ADDRESS:
		 
		sctp_process_hn_param(asoc, param, chunk, err_chunk);
		retval = SCTP_IERROR_ABORT;
		break;

	case SCTP_PARAM_FWD_TSN_SUPPORT:
		if (net->sctp.prsctp_enable)
			break;
		goto fallthrough;

	case SCTP_PARAM_RANDOM:
		if (!ep->auth_enable)
			goto fallthrough;

		 
		if (SCTP_AUTH_RANDOM_LENGTH !=
			ntohs(param.p->length) - sizeof(sctp_paramhdr_t)) {
			sctp_process_inv_paramlength(asoc, param.p,
							chunk, err_chunk);
			retval = SCTP_IERROR_ABORT;
		}
		break;

	case SCTP_PARAM_CHUNKS:
		if (!ep->auth_enable)
			goto fallthrough;

		 
		if (260 < ntohs(param.p->length)) {
			sctp_process_inv_paramlength(asoc, param.p,
						     chunk, err_chunk);
			retval = SCTP_IERROR_ABORT;
		}
		break;

	case SCTP_PARAM_HMAC_ALGO:
		if (!ep->auth_enable)
			goto fallthrough;

		hmacs = (struct sctp_hmac_algo_param *)param.p;
		n_elt = (ntohs(param.p->length) - sizeof(sctp_paramhdr_t)) >> 1;

		 
		for (i = 0; i < n_elt; i++) {
			id = ntohs(hmacs->hmac_ids[i]);

			if (id == SCTP_AUTH_HMAC_ID_SHA1)
				break;
		}

		if (id != SCTP_AUTH_HMAC_ID_SHA1) {
			sctp_process_inv_paramlength(asoc, param.p, chunk,
						     err_chunk);
			retval = SCTP_IERROR_ABORT;
		}
		break;
fallthrough:
	default:
		pr_debug("%s: unrecognized param:%d for chunk:%d\n",
			 __func__, ntohs(param.p->type), cid);

		retval = sctp_process_unk_param(asoc, param, chunk, err_chunk);
		break;
	}
	return retval;
}