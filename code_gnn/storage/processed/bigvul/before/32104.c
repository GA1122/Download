static int dev_cpu_callback(struct notifier_block *nfb,
			    unsigned long action,
			    void *ocpu)
{
	struct sk_buff **list_skb;
	struct Qdisc **list_net;
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

	 
	list_net = &sd->output_queue;
	while (*list_net)
		list_net = &(*list_net)->next_sched;
	 
	*list_net = oldsd->output_queue;
	oldsd->output_queue = NULL;

	raise_softirq_irqoff(NET_TX_SOFTIRQ);
	local_irq_enable();

	 
	while ((skb = __skb_dequeue(&oldsd->input_pkt_queue)))
		netif_rx(skb);

	return NOTIFY_OK;
}