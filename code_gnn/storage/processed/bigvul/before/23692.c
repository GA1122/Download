static void bond_arp_send_all(struct bonding *bond, struct slave *slave)
{
	int i, vlan_id;
	__be32 *targets = bond->params.arp_targets;
	struct vlan_entry *vlan;
	struct net_device *vlan_dev;
	struct rtable *rt;

	for (i = 0; (i < BOND_MAX_ARP_TARGETS); i++) {
		if (!targets[i])
			break;
		pr_debug("basa: target %x\n", targets[i]);
		if (!bond_vlan_used(bond)) {
			pr_debug("basa: empty vlan: arp_send\n");
			bond_arp_send(slave->dev, ARPOP_REQUEST, targets[i],
				      bond->master_ip, 0);
			continue;
		}

		 
		rt = ip_route_output(dev_net(bond->dev), targets[i], 0,
				     RTO_ONLINK, 0);
		if (IS_ERR(rt)) {
			if (net_ratelimit()) {
				pr_warning("%s: no route to arp_ip_target %pI4\n",
					   bond->dev->name, &targets[i]);
			}
			continue;
		}

		 
		if (rt->dst.dev == bond->dev) {
			ip_rt_put(rt);
			pr_debug("basa: rtdev == bond->dev: arp_send\n");
			bond_arp_send(slave->dev, ARPOP_REQUEST, targets[i],
				      bond->master_ip, 0);
			continue;
		}

		vlan_id = 0;
		list_for_each_entry(vlan, &bond->vlan_list, vlan_list) {
			rcu_read_lock();
			vlan_dev = __vlan_find_dev_deep(bond->dev,
							vlan->vlan_id);
			rcu_read_unlock();
			if (vlan_dev == rt->dst.dev) {
				vlan_id = vlan->vlan_id;
				pr_debug("basa: vlan match on %s %d\n",
				       vlan_dev->name, vlan_id);
				break;
			}
		}

		if (vlan_id) {
			ip_rt_put(rt);
			bond_arp_send(slave->dev, ARPOP_REQUEST, targets[i],
				      vlan->vlan_ip, vlan_id);
			continue;
		}

		if (net_ratelimit()) {
			pr_warning("%s: no path to arp_ip_target %pI4 via rt.dev %s\n",
				   bond->dev->name, &targets[i],
				   rt->dst.dev ? rt->dst.dev->name : "NULL");
		}
		ip_rt_put(rt);
	}
}