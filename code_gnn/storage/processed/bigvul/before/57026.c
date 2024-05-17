static int sctp_side_effects(sctp_event_t event_type, sctp_subtype_t subtype,
			     sctp_state_t state,
			     struct sctp_endpoint *ep,
			     struct sctp_association *asoc,
			     void *event_arg,
			     sctp_disposition_t status,
			     sctp_cmd_seq_t *commands,
			     gfp_t gfp)
{
	int error;

	 
	if (0 != (error = sctp_cmd_interpreter(event_type, subtype, state,
					       ep, asoc,
					       event_arg, status,
					       commands, gfp)))
		goto bail;

	switch (status) {
	case SCTP_DISPOSITION_DISCARD:
		pr_debug("%s: ignored sctp protocol event - state:%d, "
			 "event_type:%d, event_id:%d\n", __func__, state,
			 event_type, subtype.chunk);
		break;

	case SCTP_DISPOSITION_NOMEM:
		 
		 
		error = -ENOMEM;
		break;

	case SCTP_DISPOSITION_DELETE_TCB:
		 
		break;

	case SCTP_DISPOSITION_CONSUME:
	case SCTP_DISPOSITION_ABORT:
		 
		break;

	case SCTP_DISPOSITION_VIOLATION:
		net_err_ratelimited("protocol violation state %d chunkid %d\n",
				    state, subtype.chunk);
		break;

	case SCTP_DISPOSITION_NOT_IMPL:
		pr_warn("unimplemented feature in state %d, event_type %d, event_id %d\n",
			state, event_type, subtype.chunk);
		break;

	case SCTP_DISPOSITION_BUG:
		pr_err("bug in state %d, event_type %d, event_id %d\n",
		       state, event_type, subtype.chunk);
		BUG();
		break;

	default:
		pr_err("impossible disposition %d in state %d, event_type %d, event_id %d\n",
		       status, state, event_type, subtype.chunk);
		BUG();
		break;
	}

bail:
	return error;
}
