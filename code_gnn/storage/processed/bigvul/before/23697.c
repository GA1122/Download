static int bond_check_params(struct bond_params *params)
{
	int arp_validate_value, fail_over_mac_value, primary_reselect_value;

	 
	if (mode) {
		bond_mode = bond_parse_parm(mode, bond_mode_tbl);
		if (bond_mode == -1) {
			pr_err("Error: Invalid bonding mode \"%s\"\n",
			       mode == NULL ? "NULL" : mode);
			return -EINVAL;
		}
	}

	if (xmit_hash_policy) {
		if ((bond_mode != BOND_MODE_XOR) &&
		    (bond_mode != BOND_MODE_8023AD)) {
			pr_info("xmit_hash_policy param is irrelevant in mode %s\n",
			       bond_mode_name(bond_mode));
		} else {
			xmit_hashtype = bond_parse_parm(xmit_hash_policy,
							xmit_hashtype_tbl);
			if (xmit_hashtype == -1) {
				pr_err("Error: Invalid xmit_hash_policy \"%s\"\n",
				       xmit_hash_policy == NULL ? "NULL" :
				       xmit_hash_policy);
				return -EINVAL;
			}
		}
	}

	if (lacp_rate) {
		if (bond_mode != BOND_MODE_8023AD) {
			pr_info("lacp_rate param is irrelevant in mode %s\n",
				bond_mode_name(bond_mode));
		} else {
			lacp_fast = bond_parse_parm(lacp_rate, bond_lacp_tbl);
			if (lacp_fast == -1) {
				pr_err("Error: Invalid lacp rate \"%s\"\n",
				       lacp_rate == NULL ? "NULL" : lacp_rate);
				return -EINVAL;
			}
		}
	}

	if (ad_select) {
		params->ad_select = bond_parse_parm(ad_select, ad_select_tbl);
		if (params->ad_select == -1) {
			pr_err("Error: Invalid ad_select \"%s\"\n",
			       ad_select == NULL ? "NULL" : ad_select);
			return -EINVAL;
		}

		if (bond_mode != BOND_MODE_8023AD) {
			pr_warning("ad_select param only affects 802.3ad mode\n");
		}
	} else {
		params->ad_select = BOND_AD_STABLE;
	}

	if (max_bonds < 0) {
		pr_warning("Warning: max_bonds (%d) not in range %d-%d, so it was reset to BOND_DEFAULT_MAX_BONDS (%d)\n",
			   max_bonds, 0, INT_MAX, BOND_DEFAULT_MAX_BONDS);
		max_bonds = BOND_DEFAULT_MAX_BONDS;
	}

	if (miimon < 0) {
		pr_warning("Warning: miimon module parameter (%d), not in range 0-%d, so it was reset to %d\n",
			   miimon, INT_MAX, BOND_LINK_MON_INTERV);
		miimon = BOND_LINK_MON_INTERV;
	}

	if (updelay < 0) {
		pr_warning("Warning: updelay module parameter (%d), not in range 0-%d, so it was reset to 0\n",
			   updelay, INT_MAX);
		updelay = 0;
	}

	if (downdelay < 0) {
		pr_warning("Warning: downdelay module parameter (%d), not in range 0-%d, so it was reset to 0\n",
			   downdelay, INT_MAX);
		downdelay = 0;
	}

	if ((use_carrier != 0) && (use_carrier != 1)) {
		pr_warning("Warning: use_carrier module parameter (%d), not of valid value (0/1), so it was set to 1\n",
			   use_carrier);
		use_carrier = 1;
	}

	if (num_peer_notif < 0 || num_peer_notif > 255) {
		pr_warning("Warning: num_grat_arp/num_unsol_na (%d) not in range 0-255 so it was reset to 1\n",
			   num_peer_notif);
		num_peer_notif = 1;
	}

	 
	if (bond_mode == BOND_MODE_8023AD) {
		if (!miimon) {
			pr_warning("Warning: miimon must be specified, otherwise bonding will not detect link failure, speed and duplex which are essential for 802.3ad operation\n");
			pr_warning("Forcing miimon to 100msec\n");
			miimon = 100;
		}
	}

	if (tx_queues < 1 || tx_queues > 255) {
		pr_warning("Warning: tx_queues (%d) should be between "
			   "1 and 255, resetting to %d\n",
			   tx_queues, BOND_DEFAULT_TX_QUEUES);
		tx_queues = BOND_DEFAULT_TX_QUEUES;
	}

	if ((all_slaves_active != 0) && (all_slaves_active != 1)) {
		pr_warning("Warning: all_slaves_active module parameter (%d), "
			   "not of valid value (0/1), so it was set to "
			   "0\n", all_slaves_active);
		all_slaves_active = 0;
	}

	if (resend_igmp < 0 || resend_igmp > 255) {
		pr_warning("Warning: resend_igmp (%d) should be between "
			   "0 and 255, resetting to %d\n",
			   resend_igmp, BOND_DEFAULT_RESEND_IGMP);
		resend_igmp = BOND_DEFAULT_RESEND_IGMP;
	}

	 
	if ((bond_mode == BOND_MODE_TLB) ||
	    (bond_mode == BOND_MODE_ALB)) {
		if (!miimon) {
			pr_warning("Warning: miimon must be specified, otherwise bonding will not detect link failure and link speed which are essential for TLB/ALB load balancing\n");
			pr_warning("Forcing miimon to 100msec\n");
			miimon = 100;
		}
	}

	if (bond_mode == BOND_MODE_ALB) {
		pr_notice("In ALB mode you might experience client disconnections upon reconnection of a link if the bonding module updelay parameter (%d msec) is incompatible with the forwarding delay time of the switch\n",
			  updelay);
	}

	if (!miimon) {
		if (updelay || downdelay) {
			 
			pr_warning("Warning: miimon module parameter not set and updelay (%d) or downdelay (%d) module parameter is set; updelay and downdelay have no effect unless miimon is set\n",
				   updelay, downdelay);
		}
	} else {
		 
		if (arp_interval) {
			pr_warning("Warning: miimon (%d) and arp_interval (%d) can't be used simultaneously, disabling ARP monitoring\n",
				   miimon, arp_interval);
			arp_interval = 0;
		}

		if ((updelay % miimon) != 0) {
			pr_warning("Warning: updelay (%d) is not a multiple of miimon (%d), updelay rounded to %d ms\n",
				   updelay, miimon,
				   (updelay / miimon) * miimon);
		}

		updelay /= miimon;

		if ((downdelay % miimon) != 0) {
			pr_warning("Warning: downdelay (%d) is not a multiple of miimon (%d), downdelay rounded to %d ms\n",
				   downdelay, miimon,
				   (downdelay / miimon) * miimon);
		}

		downdelay /= miimon;
	}

	if (arp_interval < 0) {
		pr_warning("Warning: arp_interval module parameter (%d) , not in range 0-%d, so it was reset to %d\n",
			   arp_interval, INT_MAX, BOND_LINK_ARP_INTERV);
		arp_interval = BOND_LINK_ARP_INTERV;
	}

	for (arp_ip_count = 0;
	     (arp_ip_count < BOND_MAX_ARP_TARGETS) && arp_ip_target[arp_ip_count];
	     arp_ip_count++) {
		 
		if (!isdigit(arp_ip_target[arp_ip_count][0])) {
			pr_warning("Warning: bad arp_ip_target module parameter (%s), ARP monitoring will not be performed\n",
				   arp_ip_target[arp_ip_count]);
			arp_interval = 0;
		} else {
			__be32 ip = in_aton(arp_ip_target[arp_ip_count]);
			arp_target[arp_ip_count] = ip;
		}
	}

	if (arp_interval && !arp_ip_count) {
		 
		pr_warning("Warning: arp_interval module parameter (%d) specified without providing an arp_ip_target parameter, arp_interval was reset to 0\n",
			   arp_interval);
		arp_interval = 0;
	}

	if (arp_validate) {
		if (bond_mode != BOND_MODE_ACTIVEBACKUP) {
			pr_err("arp_validate only supported in active-backup mode\n");
			return -EINVAL;
		}
		if (!arp_interval) {
			pr_err("arp_validate requires arp_interval\n");
			return -EINVAL;
		}

		arp_validate_value = bond_parse_parm(arp_validate,
						     arp_validate_tbl);
		if (arp_validate_value == -1) {
			pr_err("Error: invalid arp_validate \"%s\"\n",
			       arp_validate == NULL ? "NULL" : arp_validate);
			return -EINVAL;
		}
	} else
		arp_validate_value = 0;

	if (miimon) {
		pr_info("MII link monitoring set to %d ms\n", miimon);
	} else if (arp_interval) {
		int i;

		pr_info("ARP monitoring set to %d ms, validate %s, with %d target(s):",
			arp_interval,
			arp_validate_tbl[arp_validate_value].modename,
			arp_ip_count);

		for (i = 0; i < arp_ip_count; i++)
			pr_info(" %s", arp_ip_target[i]);

		pr_info("\n");

	} else if (max_bonds) {
		 
		pr_warning("Warning: either miimon or arp_interval and arp_ip_target module parameters must be specified, otherwise bonding will not detect link failures! see bonding.txt for details.\n");
	}

	if (primary && !USES_PRIMARY(bond_mode)) {
		 
		pr_warning("Warning: %s primary device specified but has no effect in %s mode\n",
			   primary, bond_mode_name(bond_mode));
		primary = NULL;
	}

	if (primary && primary_reselect) {
		primary_reselect_value = bond_parse_parm(primary_reselect,
							 pri_reselect_tbl);
		if (primary_reselect_value == -1) {
			pr_err("Error: Invalid primary_reselect \"%s\"\n",
			       primary_reselect ==
					NULL ? "NULL" : primary_reselect);
			return -EINVAL;
		}
	} else {
		primary_reselect_value = BOND_PRI_RESELECT_ALWAYS;
	}

	if (fail_over_mac) {
		fail_over_mac_value = bond_parse_parm(fail_over_mac,
						      fail_over_mac_tbl);
		if (fail_over_mac_value == -1) {
			pr_err("Error: invalid fail_over_mac \"%s\"\n",
			       arp_validate == NULL ? "NULL" : arp_validate);
			return -EINVAL;
		}

		if (bond_mode != BOND_MODE_ACTIVEBACKUP)
			pr_warning("Warning: fail_over_mac only affects active-backup mode.\n");
	} else {
		fail_over_mac_value = BOND_FOM_NONE;
	}

	 
	params->mode = bond_mode;
	params->xmit_policy = xmit_hashtype;
	params->miimon = miimon;
	params->num_peer_notif = num_peer_notif;
	params->arp_interval = arp_interval;
	params->arp_validate = arp_validate_value;
	params->updelay = updelay;
	params->downdelay = downdelay;
	params->use_carrier = use_carrier;
	params->lacp_fast = lacp_fast;
	params->primary[0] = 0;
	params->primary_reselect = primary_reselect_value;
	params->fail_over_mac = fail_over_mac_value;
	params->tx_queues = tx_queues;
	params->all_slaves_active = all_slaves_active;
	params->resend_igmp = resend_igmp;
	params->min_links = min_links;

	if (primary) {
		strncpy(params->primary, primary, IFNAMSIZ);
		params->primary[IFNAMSIZ - 1] = 0;
	}

	memcpy(params->arp_targets, arp_target, sizeof(arp_target));

	return 0;
}