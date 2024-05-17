 int __ref online_pages(unsigned long pfn, unsigned long nr_pages)
 {
 	unsigned long onlined_pages = 0;
 	struct zone *zone;
 	int need_zonelists_rebuild = 0;
 	int nid;
 	int ret;
 	struct memory_notify arg;
 
 	lock_memory_hotplug();
 	arg.start_pfn = pfn;
 	arg.nr_pages = nr_pages;
 	arg.status_change_nid = -1;
 
 	nid = page_to_nid(pfn_to_page(pfn));
 	if (node_present_pages(nid) == 0)
 		arg.status_change_nid = nid;
 
 	ret = memory_notify(MEM_GOING_ONLINE, &arg);
 	ret = notifier_to_errno(ret);
 	if (ret) {
 		memory_notify(MEM_CANCEL_ONLINE, &arg);
 		unlock_memory_hotplug();
 		return ret;
 	}
 	 
 	zone = page_zone(pfn_to_page(pfn));
 	 
 	mutex_lock(&zonelists_mutex);
 	if (!populated_zone(zone))
 		need_zonelists_rebuild = 1;
 
 	ret = walk_system_ram_range(pfn, nr_pages, &onlined_pages,
 		online_pages_range);
 	if (ret) {
 		mutex_unlock(&zonelists_mutex);
 		printk(KERN_DEBUG "online_pages [mem %#010llx-%#010llx] failed\n",
 		       (unsigned long long) pfn << PAGE_SHIFT,
 		       (((unsigned long long) pfn + nr_pages)
 			    << PAGE_SHIFT) - 1);
 		memory_notify(MEM_CANCEL_ONLINE, &arg);
 		unlock_memory_hotplug();
 		return ret;
 	}
  
  	zone->present_pages += onlined_pages;
  	zone->zone_pgdat->node_present_pages += onlined_pages;
	if (need_zonelists_rebuild)
		build_all_zonelists(NULL, zone);
	else
		zone_pcp_update(zone);
// 	if (onlined_pages) {
// 		node_set_state(zone_to_nid(zone), N_HIGH_MEMORY);
// 		if (need_zonelists_rebuild)
// 			build_all_zonelists(NULL, zone);
// 		else
// 			zone_pcp_update(zone);
// 	}
  
  	mutex_unlock(&zonelists_mutex);
  
  	init_per_zone_wmark_min();
  
	if (onlined_pages) {
// 	if (onlined_pages)
  		kswapd_run(zone_to_nid(zone));
		node_set_state(zone_to_nid(zone), N_HIGH_MEMORY);
	}
  
  	vm_total_pages = nr_free_pagecache_pages();
  
 	writeback_set_ratelimit();
 
 	if (onlined_pages)
 		memory_notify(MEM_ONLINE, &arg);
 	unlock_memory_hotplug();
 
 	return 0;
 }