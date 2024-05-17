int netif_set_xps_queue(struct net_device *dev, const struct cpumask *mask,
			u16 index)
{
	struct xps_dev_maps *dev_maps, *new_dev_maps = NULL;
	struct xps_map *map, *new_map;
	int maps_sz = max_t(unsigned int, XPS_DEV_MAPS_SIZE, L1_CACHE_BYTES);
	int cpu, numa_node_id = -2;
	bool active = false;

	mutex_lock(&xps_map_mutex);

	dev_maps = xmap_dereference(dev->xps_maps);

	 
	for_each_online_cpu(cpu) {
		if (!cpumask_test_cpu(cpu, mask))
			continue;

		if (!new_dev_maps)
			new_dev_maps = kzalloc(maps_sz, GFP_KERNEL);
		if (!new_dev_maps) {
			mutex_unlock(&xps_map_mutex);
			return -ENOMEM;
		}

		map = dev_maps ? xmap_dereference(dev_maps->cpu_map[cpu]) :
				 NULL;

		map = expand_xps_map(map, cpu, index);
		if (!map)
			goto error;

		RCU_INIT_POINTER(new_dev_maps->cpu_map[cpu], map);
	}

	if (!new_dev_maps)
		goto out_no_new_maps;

	for_each_possible_cpu(cpu) {
		if (cpumask_test_cpu(cpu, mask) && cpu_online(cpu)) {
			 
			int pos = 0;

			map = xmap_dereference(new_dev_maps->cpu_map[cpu]);
			while ((pos < map->len) && (map->queues[pos] != index))
				pos++;

			if (pos == map->len)
				map->queues[map->len++] = index;
#ifdef CONFIG_NUMA
			if (numa_node_id == -2)
				numa_node_id = cpu_to_node(cpu);
			else if (numa_node_id != cpu_to_node(cpu))
				numa_node_id = -1;
#endif
		} else if (dev_maps) {
			 
			map = xmap_dereference(dev_maps->cpu_map[cpu]);
			RCU_INIT_POINTER(new_dev_maps->cpu_map[cpu], map);
		}

	}

	rcu_assign_pointer(dev->xps_maps, new_dev_maps);

	 
	if (dev_maps) {
		for_each_possible_cpu(cpu) {
			new_map = xmap_dereference(new_dev_maps->cpu_map[cpu]);
			map = xmap_dereference(dev_maps->cpu_map[cpu]);
			if (map && map != new_map)
				kfree_rcu(map, rcu);
		}

		kfree_rcu(dev_maps, rcu);
	}

	dev_maps = new_dev_maps;
	active = true;

out_no_new_maps:
	 
	netdev_queue_numa_node_write(netdev_get_tx_queue(dev, index),
				     (numa_node_id >= 0) ? numa_node_id :
				     NUMA_NO_NODE);

	if (!dev_maps)
		goto out_no_maps;

	 
	for_each_possible_cpu(cpu) {
		if (cpumask_test_cpu(cpu, mask) && cpu_online(cpu))
			continue;

		if (remove_xps_queue(dev_maps, cpu, index))
			active = true;
	}

	 
	if (!active) {
		RCU_INIT_POINTER(dev->xps_maps, NULL);
		kfree_rcu(dev_maps, rcu);
	}

out_no_maps:
	mutex_unlock(&xps_map_mutex);

	return 0;
error:
	 
	for_each_possible_cpu(cpu) {
		new_map = xmap_dereference(new_dev_maps->cpu_map[cpu]);
		map = dev_maps ? xmap_dereference(dev_maps->cpu_map[cpu]) :
				 NULL;
		if (new_map && new_map != map)
			kfree(new_map);
	}

	mutex_unlock(&xps_map_mutex);

	kfree(new_dev_maps);
	return -ENOMEM;
}