 int hsr_dev_finalize(struct net_device *hsr_dev, struct net_device *slave[2],
 		     unsigned char multicast_spec, u8 protocol_version)
 {
 	struct hsr_priv *hsr;
 	struct hsr_port *port;
 	int res;
 
 	hsr = netdev_priv(hsr_dev);
 	INIT_LIST_HEAD(&hsr->ports);
 	INIT_LIST_HEAD(&hsr->node_db);
 	INIT_LIST_HEAD(&hsr->self_node_db);
 
 	ether_addr_copy(hsr_dev->dev_addr, slave[0]->dev_addr);
 
 	 
 	res = hsr_create_self_node(&hsr->self_node_db, hsr_dev->dev_addr,
 				   slave[1]->dev_addr);
 	if (res < 0)
 		return res;
 
 	spin_lock_init(&hsr->seqnr_lock);
 	 
 	hsr->sequence_nr = HSR_SEQNR_START;
 	hsr->sup_sequence_nr = HSR_SUP_SEQNR_START;
 
 	timer_setup(&hsr->announce_timer, hsr_announce, 0);
 	timer_setup(&hsr->prune_timer, hsr_prune_nodes, 0);
 
 	ether_addr_copy(hsr->sup_multicast_addr, def_multicast_addr);
 	hsr->sup_multicast_addr[ETH_ALEN - 1] = multicast_spec;
 
 	hsr->protVersion = protocol_version;
 
 	 
 
 	 
 	netif_carrier_off(hsr_dev);
  
  	res = hsr_add_port(hsr, hsr_dev, HSR_PT_MASTER);
  	if (res)
		return res;
// 		goto err_add_port;
  
  	res = register_netdevice(hsr_dev);
  	if (res)
 		goto fail;
 
 	res = hsr_add_port(hsr, slave[0], HSR_PT_SLAVE_A);
 	if (res)
 		goto fail;
 	res = hsr_add_port(hsr, slave[1], HSR_PT_SLAVE_B);
 	if (res)
 		goto fail;
 
 	mod_timer(&hsr->prune_timer, jiffies + msecs_to_jiffies(PRUNE_PERIOD));
 
 	return 0;
 
  fail:
  	hsr_for_each_port(hsr, port)
  		hsr_del_port(port);
// err_add_port:
// 	hsr_del_node(&hsr->self_node_db);
  
  	return res;
  }