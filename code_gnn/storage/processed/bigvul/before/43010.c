static void virtnet_set_affinity(struct virtnet_info *vi)
{
	int i;
	int cpu;

	 
	if (vi->curr_queue_pairs == 1 ||
	    vi->max_queue_pairs != num_online_cpus()) {
		virtnet_clean_affinity(vi, -1);
		return;
	}

	i = 0;
	for_each_online_cpu(cpu) {
		virtqueue_set_affinity(vi->rq[i].vq, cpu);
		virtqueue_set_affinity(vi->sq[i].vq, cpu);
		netif_set_xps_queue(vi->dev, cpumask_of(cpu), i);
		i++;
	}

	vi->affinity_hint_set = true;
}