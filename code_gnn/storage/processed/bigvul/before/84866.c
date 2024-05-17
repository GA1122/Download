unsigned int ebt_do_table(struct sk_buff *skb,
			  const struct nf_hook_state *state,
			  struct ebt_table *table)
{
	unsigned int hook = state->hook;
	int i, nentries;
	struct ebt_entry *point;
	struct ebt_counter *counter_base, *cb_base;
	const struct ebt_entry_target *t;
	int verdict, sp = 0;
	struct ebt_chainstack *cs;
	struct ebt_entries *chaininfo;
	const char *base;
	const struct ebt_table_info *private;
	struct xt_action_param acpar;

	acpar.state   = state;
	acpar.hotdrop = false;

	read_lock_bh(&table->lock);
	private = table->private;
	cb_base = COUNTER_BASE(private->counters, private->nentries,
	   smp_processor_id());
	if (private->chainstack)
		cs = private->chainstack[smp_processor_id()];
	else
		cs = NULL;
	chaininfo = private->hook_entry[hook];
	nentries = private->hook_entry[hook]->nentries;
	point = (struct ebt_entry *)(private->hook_entry[hook]->data);
	counter_base = cb_base + private->hook_entry[hook]->counter_offset;
	 
	base = private->entries;
	i = 0;
	while (i < nentries) {
		if (ebt_basic_match(point, skb, state->in, state->out))
			goto letscontinue;

		if (EBT_MATCH_ITERATE(point, ebt_do_match, skb, &acpar) != 0)
			goto letscontinue;
		if (acpar.hotdrop) {
			read_unlock_bh(&table->lock);
			return NF_DROP;
		}

		 
		(*(counter_base + i)).pcnt++;
		(*(counter_base + i)).bcnt += skb->len;

		 
		EBT_WATCHER_ITERATE(point, ebt_do_watcher, skb, &acpar);

		t = (struct ebt_entry_target *)
		   (((char *)point) + point->target_offset);
		 
		if (!t->u.target->target)
			verdict = ((struct ebt_standard_target *)t)->verdict;
		else {
			acpar.target   = t->u.target;
			acpar.targinfo = t->data;
			verdict = t->u.target->target(skb, &acpar);
		}
		if (verdict == EBT_ACCEPT) {
			read_unlock_bh(&table->lock);
			return NF_ACCEPT;
		}
		if (verdict == EBT_DROP) {
			read_unlock_bh(&table->lock);
			return NF_DROP;
		}
		if (verdict == EBT_RETURN) {
letsreturn:
			if (WARN(sp == 0, "RETURN on base chain")) {
				 
				goto letscontinue;
			}

			sp--;
			 
			i = cs[sp].n;
			chaininfo = cs[sp].chaininfo;
			nentries = chaininfo->nentries;
			point = cs[sp].e;
			counter_base = cb_base +
			   chaininfo->counter_offset;
			continue;
		}
		if (verdict == EBT_CONTINUE)
			goto letscontinue;

		if (WARN(verdict < 0, "bogus standard verdict\n")) {
			read_unlock_bh(&table->lock);
			return NF_DROP;
		}

		 
		cs[sp].n = i + 1;
		cs[sp].chaininfo = chaininfo;
		cs[sp].e = ebt_next_entry(point);
		i = 0;
		chaininfo = (struct ebt_entries *) (base + verdict);

		if (WARN(chaininfo->distinguisher, "jump to non-chain\n")) {
			read_unlock_bh(&table->lock);
			return NF_DROP;
		}

		nentries = chaininfo->nentries;
		point = (struct ebt_entry *)chaininfo->data;
		counter_base = cb_base + chaininfo->counter_offset;
		sp++;
		continue;
letscontinue:
		point = ebt_next_entry(point);
		i++;
	}

	 
	if (chaininfo->policy == EBT_RETURN)
		goto letsreturn;
	if (chaininfo->policy == EBT_ACCEPT) {
		read_unlock_bh(&table->lock);
		return NF_ACCEPT;
	}
	read_unlock_bh(&table->lock);
	return NF_DROP;
}