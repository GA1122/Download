ar6000_targetStats_event(struct ar6_softc *ar,  u8 *ptr, u32 len)
{
    u8 ac;

    if(ar->arNetworkType == AP_NETWORK) {
        WMI_AP_MODE_STAT *p = (WMI_AP_MODE_STAT *)ptr;
        WMI_AP_MODE_STAT *ap = &ar->arAPStats;

        if (len < sizeof(*p)) {
            return;
        }

        for(ac=0;ac<AP_MAX_NUM_STA;ac++) {
            ap->sta[ac].tx_bytes   += p->sta[ac].tx_bytes;
            ap->sta[ac].tx_pkts    += p->sta[ac].tx_pkts;
            ap->sta[ac].tx_error   += p->sta[ac].tx_error;
            ap->sta[ac].tx_discard += p->sta[ac].tx_discard;
            ap->sta[ac].rx_bytes   += p->sta[ac].rx_bytes;
            ap->sta[ac].rx_pkts    += p->sta[ac].rx_pkts;
            ap->sta[ac].rx_error   += p->sta[ac].rx_error;
            ap->sta[ac].rx_discard += p->sta[ac].rx_discard;
        }

    } else {
        WMI_TARGET_STATS *pTarget = (WMI_TARGET_STATS *)ptr;
         TARGET_STATS *pStats = &ar->arTargetStats;

        if (len < sizeof(*pTarget)) {
            return;
        }

        if (ar->arConnected) {
            bss_t *pConnBss = NULL;

            pConnBss = wmi_find_node(ar->arWmi,ar->arBssid);
            if (pConnBss)
            {
                pConnBss->ni_rssi = pTarget->cservStats.cs_aveBeacon_rssi;
                pConnBss->ni_snr = pTarget->cservStats.cs_aveBeacon_snr;
                wmi_node_return(ar->arWmi, pConnBss);
            }
        }

        AR_DEBUG_PRINTF(ATH_DEBUG_INFO,("AR6000 updating target stats\n"));
        pStats->tx_packets          += pTarget->txrxStats.tx_stats.tx_packets;
        pStats->tx_bytes            += pTarget->txrxStats.tx_stats.tx_bytes;
        pStats->tx_unicast_pkts     += pTarget->txrxStats.tx_stats.tx_unicast_pkts;
        pStats->tx_unicast_bytes    += pTarget->txrxStats.tx_stats.tx_unicast_bytes;
        pStats->tx_multicast_pkts   += pTarget->txrxStats.tx_stats.tx_multicast_pkts;
        pStats->tx_multicast_bytes  += pTarget->txrxStats.tx_stats.tx_multicast_bytes;
        pStats->tx_broadcast_pkts   += pTarget->txrxStats.tx_stats.tx_broadcast_pkts;
        pStats->tx_broadcast_bytes  += pTarget->txrxStats.tx_stats.tx_broadcast_bytes;
        pStats->tx_rts_success_cnt  += pTarget->txrxStats.tx_stats.tx_rts_success_cnt;
        for(ac = 0; ac < WMM_NUM_AC; ac++)
            pStats->tx_packet_per_ac[ac] += pTarget->txrxStats.tx_stats.tx_packet_per_ac[ac];
        pStats->tx_errors           += pTarget->txrxStats.tx_stats.tx_errors;
        pStats->tx_failed_cnt       += pTarget->txrxStats.tx_stats.tx_failed_cnt;
        pStats->tx_retry_cnt        += pTarget->txrxStats.tx_stats.tx_retry_cnt;
        pStats->tx_mult_retry_cnt   += pTarget->txrxStats.tx_stats.tx_mult_retry_cnt;
        pStats->tx_rts_fail_cnt     += pTarget->txrxStats.tx_stats.tx_rts_fail_cnt;
        pStats->tx_unicast_rate      = wmi_get_rate(pTarget->txrxStats.tx_stats.tx_unicast_rate);

        pStats->rx_packets          += pTarget->txrxStats.rx_stats.rx_packets;
        pStats->rx_bytes            += pTarget->txrxStats.rx_stats.rx_bytes;
        pStats->rx_unicast_pkts     += pTarget->txrxStats.rx_stats.rx_unicast_pkts;
        pStats->rx_unicast_bytes    += pTarget->txrxStats.rx_stats.rx_unicast_bytes;
        pStats->rx_multicast_pkts   += pTarget->txrxStats.rx_stats.rx_multicast_pkts;
        pStats->rx_multicast_bytes  += pTarget->txrxStats.rx_stats.rx_multicast_bytes;
        pStats->rx_broadcast_pkts   += pTarget->txrxStats.rx_stats.rx_broadcast_pkts;
        pStats->rx_broadcast_bytes  += pTarget->txrxStats.rx_stats.rx_broadcast_bytes;
        pStats->rx_fragment_pkt     += pTarget->txrxStats.rx_stats.rx_fragment_pkt;
        pStats->rx_errors           += pTarget->txrxStats.rx_stats.rx_errors;
        pStats->rx_crcerr           += pTarget->txrxStats.rx_stats.rx_crcerr;
        pStats->rx_key_cache_miss   += pTarget->txrxStats.rx_stats.rx_key_cache_miss;
        pStats->rx_decrypt_err      += pTarget->txrxStats.rx_stats.rx_decrypt_err;
        pStats->rx_duplicate_frames += pTarget->txrxStats.rx_stats.rx_duplicate_frames;
        pStats->rx_unicast_rate      = wmi_get_rate(pTarget->txrxStats.rx_stats.rx_unicast_rate);


        pStats->tkip_local_mic_failure
                                += pTarget->txrxStats.tkipCcmpStats.tkip_local_mic_failure;
        pStats->tkip_counter_measures_invoked
                                += pTarget->txrxStats.tkipCcmpStats.tkip_counter_measures_invoked;
        pStats->tkip_replays        += pTarget->txrxStats.tkipCcmpStats.tkip_replays;
        pStats->tkip_format_errors  += pTarget->txrxStats.tkipCcmpStats.tkip_format_errors;
        pStats->ccmp_format_errors  += pTarget->txrxStats.tkipCcmpStats.ccmp_format_errors;
        pStats->ccmp_replays        += pTarget->txrxStats.tkipCcmpStats.ccmp_replays;

        pStats->power_save_failure_cnt += pTarget->pmStats.power_save_failure_cnt;
        pStats->noise_floor_calibation = pTarget->noise_floor_calibation;

        pStats->cs_bmiss_cnt        += pTarget->cservStats.cs_bmiss_cnt;
        pStats->cs_lowRssi_cnt      += pTarget->cservStats.cs_lowRssi_cnt;
        pStats->cs_connect_cnt      += pTarget->cservStats.cs_connect_cnt;
        pStats->cs_disconnect_cnt   += pTarget->cservStats.cs_disconnect_cnt;
        pStats->cs_aveBeacon_snr    = pTarget->cservStats.cs_aveBeacon_snr;
        pStats->cs_aveBeacon_rssi   = pTarget->cservStats.cs_aveBeacon_rssi;

        if (enablerssicompensation) {
            pStats->cs_aveBeacon_rssi =
                    rssi_compensation_calc(ar, pStats->cs_aveBeacon_rssi);
        }
        pStats->cs_lastRoam_msec    = pTarget->cservStats.cs_lastRoam_msec;
        pStats->cs_snr              = pTarget->cservStats.cs_snr;
        pStats->cs_rssi             = pTarget->cservStats.cs_rssi;

        pStats->lq_val              = pTarget->lqVal;

        pStats->wow_num_pkts_dropped += pTarget->wowStats.wow_num_pkts_dropped;
        pStats->wow_num_host_pkt_wakeups += pTarget->wowStats.wow_num_host_pkt_wakeups;
        pStats->wow_num_host_event_wakeups += pTarget->wowStats.wow_num_host_event_wakeups;
        pStats->wow_num_events_discarded += pTarget->wowStats.wow_num_events_discarded;
        pStats->arp_received += pTarget->arpStats.arp_received;
        pStats->arp_matched  += pTarget->arpStats.arp_matched;
        pStats->arp_replied  += pTarget->arpStats.arp_replied;

        if (ar->statsUpdatePending) {
            ar->statsUpdatePending = false;
            wake_up(&arEvent);
        }
    }
}