static void tg3_timer(unsigned long __opaque)
{
	struct tg3 *tp = (struct tg3 *) __opaque;

	if (tp->irq_sync || tg3_flag(tp, RESET_TASK_PENDING))
		goto restart_timer;

	spin_lock(&tp->lock);

	if (tg3_asic_rev(tp) == ASIC_REV_5717 ||
	    tg3_flag(tp, 57765_CLASS))
		tg3_chk_missed_msi(tp);

	if (tg3_flag(tp, FLUSH_POSTED_WRITES)) {
		 
		tr32(HOSTCC_MODE);
	}

	if (!tg3_flag(tp, TAGGED_STATUS)) {
		 
		if (tp->napi[0].hw_status->status & SD_STATUS_UPDATED) {
			tw32(GRC_LOCAL_CTRL,
			     tp->grc_local_ctrl | GRC_LCLCTRL_SETINT);
		} else {
			tw32(HOSTCC_MODE, tp->coalesce_mode |
			     HOSTCC_MODE_ENABLE | HOSTCC_MODE_NOW);
		}

		if (!(tr32(WDMAC_MODE) & WDMAC_MODE_ENABLE)) {
			spin_unlock(&tp->lock);
			tg3_reset_task_schedule(tp);
			goto restart_timer;
		}
	}

	 
	if (!--tp->timer_counter) {
		if (tg3_flag(tp, 5705_PLUS))
			tg3_periodic_fetch_stats(tp);

		if (tp->setlpicnt && !--tp->setlpicnt)
			tg3_phy_eee_enable(tp);

		if (tg3_flag(tp, USE_LINKCHG_REG)) {
			u32 mac_stat;
			int phy_event;

			mac_stat = tr32(MAC_STATUS);

			phy_event = 0;
			if (tp->phy_flags & TG3_PHYFLG_USE_MI_INTERRUPT) {
				if (mac_stat & MAC_STATUS_MI_INTERRUPT)
					phy_event = 1;
			} else if (mac_stat & MAC_STATUS_LNKSTATE_CHANGED)
				phy_event = 1;

			if (phy_event)
				tg3_setup_phy(tp, 0);
		} else if (tg3_flag(tp, POLL_SERDES)) {
			u32 mac_stat = tr32(MAC_STATUS);
			int need_setup = 0;

			if (tp->link_up &&
			    (mac_stat & MAC_STATUS_LNKSTATE_CHANGED)) {
				need_setup = 1;
			}
			if (!tp->link_up &&
			    (mac_stat & (MAC_STATUS_PCS_SYNCED |
					 MAC_STATUS_SIGNAL_DET))) {
				need_setup = 1;
			}
			if (need_setup) {
				if (!tp->serdes_counter) {
					tw32_f(MAC_MODE,
					     (tp->mac_mode &
					      ~MAC_MODE_PORT_MODE_MASK));
					udelay(40);
					tw32_f(MAC_MODE, tp->mac_mode);
					udelay(40);
				}
				tg3_setup_phy(tp, 0);
			}
		} else if ((tp->phy_flags & TG3_PHYFLG_MII_SERDES) &&
			   tg3_flag(tp, 5780_CLASS)) {
			tg3_serdes_parallel_detect(tp);
		}

		tp->timer_counter = tp->timer_multiplier;
	}

	 
	if (!--tp->asf_counter) {
		if (tg3_flag(tp, ENABLE_ASF) && !tg3_flag(tp, ENABLE_APE)) {
			tg3_wait_for_event_ack(tp);

			tg3_write_mem(tp, NIC_SRAM_FW_CMD_MBOX,
				      FWCMD_NICDRV_ALIVE3);
			tg3_write_mem(tp, NIC_SRAM_FW_CMD_LEN_MBOX, 4);
			tg3_write_mem(tp, NIC_SRAM_FW_CMD_DATA_MBOX,
				      TG3_FW_UPDATE_TIMEOUT_SEC);

			tg3_generate_fw_event(tp);
		}
		tp->asf_counter = tp->asf_multiplier;
	}

	spin_unlock(&tp->lock);

restart_timer:
	tp->timer.expires = jiffies + tp->timer_offset;
	add_timer(&tp->timer);
}
