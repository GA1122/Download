int sctp_do_sm(struct net *net, sctp_event_t event_type, sctp_subtype_t subtype,
	       sctp_state_t state,
	       struct sctp_endpoint *ep,
	       struct sctp_association *asoc,
	       void *event_arg,
	       gfp_t gfp)
{
	sctp_cmd_seq_t commands;
	const sctp_sm_table_entry_t *state_fn;
	sctp_disposition_t status;
	int error = 0;
	typedef const char *(printfn_t)(sctp_subtype_t);
	static printfn_t *table[] = {
		NULL, sctp_cname, sctp_tname, sctp_oname, sctp_pname,
	};
	printfn_t *debug_fn  __attribute__ ((unused)) = table[event_type];

	 
	state_fn = sctp_sm_lookup_event(net, event_type, state, subtype);

	sctp_init_cmd_seq(&commands);

	debug_pre_sfn();
	status = state_fn->fn(net, ep, asoc, subtype, event_arg, &commands);
	debug_post_sfn();

	error = sctp_side_effects(event_type, subtype, state,
				  ep, asoc, event_arg, status,
				  &commands, gfp);
	debug_post_sfx();

	return error;
}