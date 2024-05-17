static int dev_cpu_callback(struct notifier_block *nfb,
			    unsigned long action,
			    void *ocpu)
{
	struct sk_buff **list_skb;
	struct sk_buff *skb;
	unsigned int cpu, oldcpu = (unsigned long)ocpu;
	struct softnet_data *sd, *oldsd;

	if (action != CPU_DEAD && action != CPU_DEAD_FROZEN)
		return NOTIFY_OK;

	local_irq_disable();
	cpu = smp_processor_id();
	sd = &per_cpu(softnet_data, cpu);
	oldsd = &per_cpu(softnet_data, oldcpu);

	 
	list_skb = &sd->completion_queue;
	while (*list_skb)
		list_skb = &(*list_skb)->next;
	 
	*list_skb = oldsd->completion_queue;
	oldsd->completion_queue = NULL;

	 
	if (oldsd->output_queue) {
		*sd->output_queue_tailp = oldsd->output_queue;
		sd->output_queue_tailp = oldsd->output_queue_tailp;
		oldsd->output_queue = NULL;
		oldsd->output_queue_tailp = &oldsd->output_queue;
	}
	 
	while (!list_empty(&oldsd->poll_list)) {
		struct napi_struct *napi = list_first_entry(&oldsd->poll_list,
							    struct napi_struct,
							    poll_list);

		list_del_init(&napi->poll_list);
		if (napi->poll == process_backlog)
			napi->state = 0;
		else
			____napi_schedule(sd, napi);
	}

	raise_softirq_irqoff(NET_TX_SOFTIRQ);
	local_irq_enable();

	 
	while ((skb = __skb_dequeue(&oldsd->process_queue))) {
		netif_rx_ni(skb);
		input_queue_head_incr(oldsd);
	}
	while ((skb = skb_dequeue(&oldsd->input_pkt_queue))) {
		netif_rx_ni(skb);
		input_queue_head_incr(oldsd);
	}

	return NOTIFY_OK;
}