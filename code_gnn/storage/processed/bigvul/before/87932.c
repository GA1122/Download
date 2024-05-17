static void fm10k_init_reta(struct fm10k_intfc *interface)
{
	u16 i, rss_i = interface->ring_feature[RING_F_RSS].indices;
	u32 reta;

	 
	if (netif_is_rxfh_configured(interface->netdev)) {
		for (i = FM10K_RETA_SIZE; i--;) {
			reta = interface->reta[i];
			if ((((reta << 24) >> 24) < rss_i) &&
			    (((reta << 16) >> 24) < rss_i) &&
			    (((reta <<  8) >> 24) < rss_i) &&
			    (((reta)       >> 24) < rss_i))
				continue;

			 
			dev_err(&interface->pdev->dev,
				"RSS indirection table assigned flows out of queue bounds. Reconfiguring.\n");
			goto repopulate_reta;
		}

		 
		return;
	}

repopulate_reta:
	fm10k_write_reta(interface, NULL);
}
