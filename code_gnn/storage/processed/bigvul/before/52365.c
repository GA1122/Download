ip6t_do_table(struct sk_buff *skb,
	      const struct nf_hook_state *state,
	      struct xt_table *table)
{
	unsigned int hook = state->hook;
	static const char nulldevname[IFNAMSIZ] __attribute__((aligned(sizeof(long))));
	 
	unsigned int verdict = NF_DROP;
	const char *indev, *outdev;
	const void *table_base;
	struct ip6t_entry *e, **jumpstack;
	unsigned int stackidx, cpu;
	const struct xt_table_info *private;
	struct xt_action_param acpar;
	unsigned int addend;

	 
	stackidx = 0;
	indev = state->in ? state->in->name : nulldevname;
	outdev = state->out ? state->out->name : nulldevname;
	 
	acpar.hotdrop = false;
	acpar.net     = state->net;
	acpar.in      = state->in;
	acpar.out     = state->out;
	acpar.family  = NFPROTO_IPV6;
	acpar.hooknum = hook;

	IP_NF_ASSERT(table->valid_hooks & (1 << hook));

	local_bh_disable();
	addend = xt_write_recseq_begin();
	private = table->private;
	 
	smp_read_barrier_depends();
	cpu        = smp_processor_id();
	table_base = private->entries;
	jumpstack  = (struct ip6t_entry **)private->jumpstack[cpu];

	 
	if (static_key_false(&xt_tee_enabled))
		jumpstack += private->stacksize * __this_cpu_read(nf_skb_duplicated);

	e = get_entry(table_base, private->hook_entry[hook]);

	do {
		const struct xt_entry_target *t;
		const struct xt_entry_match *ematch;
		struct xt_counters *counter;

		IP_NF_ASSERT(e);
		acpar.thoff = 0;
		if (!ip6_packet_match(skb, indev, outdev, &e->ipv6,
		    &acpar.thoff, &acpar.fragoff, &acpar.hotdrop)) {
 no_match:
			e = ip6t_next_entry(e);
			continue;
		}

		xt_ematch_foreach(ematch, e) {
			acpar.match     = ematch->u.kernel.match;
			acpar.matchinfo = ematch->data;
			if (!acpar.match->match(skb, &acpar))
				goto no_match;
		}

		counter = xt_get_this_cpu_counter(&e->counters);
		ADD_COUNTER(*counter, skb->len, 1);

		t = ip6t_get_target_c(e);
		IP_NF_ASSERT(t->u.kernel.target);

#if IS_ENABLED(CONFIG_NETFILTER_XT_TARGET_TRACE)
		 
		if (unlikely(skb->nf_trace))
			trace_packet(state->net, skb, hook, state->in,
				     state->out, table->name, private, e);
#endif
		 
		if (!t->u.kernel.target->target) {
			int v;

			v = ((struct xt_standard_target *)t)->verdict;
			if (v < 0) {
				 
				if (v != XT_RETURN) {
					verdict = (unsigned int)(-v) - 1;
					break;
				}
				if (stackidx == 0)
					e = get_entry(table_base,
					    private->underflow[hook]);
				else
					e = ip6t_next_entry(jumpstack[--stackidx]);
				continue;
			}
			if (table_base + v != ip6t_next_entry(e) &&
			    !(e->ipv6.flags & IP6T_F_GOTO)) {
				jumpstack[stackidx++] = e;
			}

			e = get_entry(table_base, v);
			continue;
		}

		acpar.target   = t->u.kernel.target;
		acpar.targinfo = t->data;

		verdict = t->u.kernel.target->target(skb, &acpar);
		if (verdict == XT_CONTINUE)
			e = ip6t_next_entry(e);
		else
			 
			break;
	} while (!acpar.hotdrop);

	xt_write_recseq_end(addend);
	local_bh_enable();

#ifdef DEBUG_ALLOW_ALL
	return NF_ACCEPT;
#else
	if (acpar.hotdrop)
		return NF_DROP;
	else return verdict;
#endif
}