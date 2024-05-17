static __latent_entropy void net_tx_action(struct softirq_action *h)
{
	struct softnet_data *sd = this_cpu_ptr(&softnet_data);

	if (sd->completion_queue) {
		struct sk_buff *clist;

		local_irq_disable();
		clist = sd->completion_queue;
		sd->completion_queue = NULL;
		local_irq_enable();

		while (clist) {
			struct sk_buff *skb = clist;

			clist = clist->next;

			WARN_ON(refcount_read(&skb->users));
			if (likely(get_kfree_skb_cb(skb)->reason == SKB_REASON_CONSUMED))
				trace_consume_skb(skb);
			else
				trace_kfree_skb(skb, net_tx_action);

			if (skb->fclone != SKB_FCLONE_UNAVAILABLE)
				__kfree_skb(skb);
			else
				__kfree_skb_defer(skb);
		}

		__kfree_skb_flush();
	}

	if (sd->output_queue) {
		struct Qdisc *head;

		local_irq_disable();
		head = sd->output_queue;
		sd->output_queue = NULL;
		sd->output_queue_tailp = &sd->output_queue;
		local_irq_enable();

		while (head) {
			struct Qdisc *q = head;
			spinlock_t *root_lock;

			head = head->next_sched;

			root_lock = qdisc_lock(q);
			spin_lock(root_lock);
			 
			smp_mb__before_atomic();
			clear_bit(__QDISC_STATE_SCHED, &q->state);
			qdisc_run(q);
			spin_unlock(root_lock);
		}
	}
}