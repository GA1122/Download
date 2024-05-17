static void airo_process_scan_results (struct airo_info *ai) {
	union iwreq_data	wrqu;
	BSSListRid bss;
	int rc;
	BSSListElement * loop_net;
	BSSListElement * tmp_net;

	 
	list_for_each_entry_safe (loop_net, tmp_net, &ai->network_list, list) {
		list_move_tail (&loop_net->list, &ai->network_free_list);
		 
		memset (loop_net, 0, sizeof (loop_net->bss));
	}

	 
	rc = PC4500_readrid(ai, ai->bssListFirst, &bss, ai->bssListRidLen, 0);
	if((rc) || (bss.index == cpu_to_le16(0xffff))) {
		 
		goto out;
	}

	 
	tmp_net = NULL;
	while((!rc) && (bss.index != cpu_to_le16(0xffff))) {
		 
		if (!list_empty(&ai->network_free_list)) {
			tmp_net = list_entry(ai->network_free_list.next,
					    BSSListElement, list);
			list_del(ai->network_free_list.next);
		}

		if (tmp_net != NULL) {
			memcpy(tmp_net, &bss, sizeof(tmp_net->bss));
			list_add_tail(&tmp_net->list, &ai->network_list);
			tmp_net = NULL;
		}

		 
		rc = PC4500_readrid(ai, ai->bssListNext,
				    &bss, ai->bssListRidLen, 0);
	}

out:
	ai->scan_timeout = 0;
	clear_bit(JOB_SCAN_RESULTS, &ai->jobs);
	up(&ai->sem);

	 
	wrqu.data.length = 0;
	wrqu.data.flags = 0;
	wireless_send_event(ai->dev, SIOCGIWSCAN, &wrqu, NULL);
}
