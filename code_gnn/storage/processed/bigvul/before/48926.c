static int process_backlog(struct napi_struct *napi, int quota)
{
	int work = 0;
	struct softnet_data *sd = container_of(napi, struct softnet_data, backlog);

	 
	if (sd_has_rps_ipi_waiting(sd)) {
		local_irq_disable();
		net_rps_action_and_irq_enable(sd);
	}

	napi->weight = weight_p;
	local_irq_disable();
	while (1) {
		struct sk_buff *skb;

		while ((skb = __skb_dequeue(&sd->process_queue))) {
			rcu_read_lock();
			local_irq_enable();
			__netif_receive_skb(skb);
			rcu_read_unlock();
			local_irq_disable();
			input_queue_head_incr(sd);
			if (++work >= quota) {
				local_irq_enable();
				return work;
			}
		}

		rps_lock(sd);
		if (skb_queue_empty(&sd->input_pkt_queue)) {
			 
			napi->state = 0;
			rps_unlock(sd);

			break;
		}

		skb_queue_splice_tail_init(&sd->input_pkt_queue,
					   &sd->process_queue);
		rps_unlock(sd);
	}
	local_irq_enable();

	return work;
}
