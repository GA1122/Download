static void vmxnet3_activate_device(VMXNET3State *s)
{
    int i;
    static const uint32_t VMXNET3_DEF_TX_THRESHOLD = 1;
    PCIDevice *d = PCI_DEVICE(s);
    hwaddr qdescr_table_pa;
    uint64_t pa;
    uint32_t size;

     
    if (!vmxnet3_verify_driver_magic(d, s->drv_shmem)) {
        VMW_ERPRN("Device configuration received from driver is invalid");
        return;
    }

     
    if (s->device_active) {
        VMW_CFPRN("Vmxnet3 device is active");
        return;
    }

    vmxnet3_adjust_by_guest_type(s);
    vmxnet3_update_features(s);
    vmxnet3_update_pm_state(s);
    vmxnet3_setup_rx_filtering(s);
     
    s->mtu = VMXNET3_READ_DRV_SHARED32(d, s->drv_shmem, devRead.misc.mtu);
    VMW_CFPRN("MTU is %u", s->mtu);

    s->max_rx_frags =
        VMXNET3_READ_DRV_SHARED16(d, s->drv_shmem, devRead.misc.maxNumRxSG);

    if (s->max_rx_frags == 0) {
        s->max_rx_frags = 1;
    }

    VMW_CFPRN("Max RX fragments is %u", s->max_rx_frags);

    s->event_int_idx =
        VMXNET3_READ_DRV_SHARED8(d, s->drv_shmem, devRead.intrConf.eventIntrIdx);
    assert(vmxnet3_verify_intx(s, s->event_int_idx));
    VMW_CFPRN("Events interrupt line is %u", s->event_int_idx);

    s->auto_int_masking =
        VMXNET3_READ_DRV_SHARED8(d, s->drv_shmem, devRead.intrConf.autoMask);
    VMW_CFPRN("Automatic interrupt masking is %d", (int)s->auto_int_masking);

    s->txq_num =
        VMXNET3_READ_DRV_SHARED8(d, s->drv_shmem, devRead.misc.numTxQueues);
    s->rxq_num =
        VMXNET3_READ_DRV_SHARED8(d, s->drv_shmem, devRead.misc.numRxQueues);

    VMW_CFPRN("Number of TX/RX queues %u/%u", s->txq_num, s->rxq_num);
    vmxnet3_validate_queues(s);

    qdescr_table_pa =
        VMXNET3_READ_DRV_SHARED64(d, s->drv_shmem, devRead.misc.queueDescPA);
    VMW_CFPRN("TX queues descriptors table is at 0x%" PRIx64, qdescr_table_pa);

     
    s->max_tx_frags = 0;

     
    for (i = 0; i < s->txq_num; i++) {
        hwaddr qdescr_pa =
            qdescr_table_pa + i * sizeof(struct Vmxnet3_TxQueueDesc);

         
        s->txq_descr[i].intr_idx =
            VMXNET3_READ_TX_QUEUE_DESCR8(d, qdescr_pa, conf.intrIdx);
        assert(vmxnet3_verify_intx(s, s->txq_descr[i].intr_idx));

        VMW_CFPRN("TX Queue %d interrupt: %d", i, s->txq_descr[i].intr_idx);

         
        pa = VMXNET3_READ_TX_QUEUE_DESCR64(d, qdescr_pa, conf.txRingBasePA);
        size = VMXNET3_READ_TX_QUEUE_DESCR32(d, qdescr_pa, conf.txRingSize);

        vmxnet3_ring_init(d, &s->txq_descr[i].tx_ring, pa, size,
                          sizeof(struct Vmxnet3_TxDesc), false);
        VMXNET3_RING_DUMP(VMW_CFPRN, "TX", i, &s->txq_descr[i].tx_ring);

        s->max_tx_frags += size;

         
        pa = VMXNET3_READ_TX_QUEUE_DESCR64(d, qdescr_pa, conf.compRingBasePA);
        size = VMXNET3_READ_TX_QUEUE_DESCR32(d, qdescr_pa, conf.compRingSize);
        vmxnet3_ring_init(d, &s->txq_descr[i].comp_ring, pa, size,
                          sizeof(struct Vmxnet3_TxCompDesc), true);
        VMXNET3_RING_DUMP(VMW_CFPRN, "TXC", i, &s->txq_descr[i].comp_ring);

        s->txq_descr[i].tx_stats_pa =
            qdescr_pa + offsetof(struct Vmxnet3_TxQueueDesc, stats);

        memset(&s->txq_descr[i].txq_stats, 0,
               sizeof(s->txq_descr[i].txq_stats));

         
        VMXNET3_WRITE_TX_QUEUE_DESCR32(d, qdescr_pa,
                                       ctrl.txThreshold,
                                       VMXNET3_DEF_TX_THRESHOLD);
    }

     
    VMW_CFPRN("Max TX fragments is %u", s->max_tx_frags);
    net_tx_pkt_init(&s->tx_pkt, PCI_DEVICE(s),
                    s->max_tx_frags, s->peer_has_vhdr);
    net_rx_pkt_init(&s->rx_pkt, s->peer_has_vhdr);

     
    for (i = 0; i < s->rxq_num; i++) {
        int j;
        hwaddr qd_pa =
            qdescr_table_pa + s->txq_num * sizeof(struct Vmxnet3_TxQueueDesc) +
            i * sizeof(struct Vmxnet3_RxQueueDesc);

         
        s->rxq_descr[i].intr_idx =
            VMXNET3_READ_TX_QUEUE_DESCR8(d, qd_pa, conf.intrIdx);
        assert(vmxnet3_verify_intx(s, s->rxq_descr[i].intr_idx));

        VMW_CFPRN("RX Queue %d interrupt: %d", i, s->rxq_descr[i].intr_idx);

         
        for (j = 0; j < VMXNET3_RX_RINGS_PER_QUEUE; j++) {
             
            pa = VMXNET3_READ_RX_QUEUE_DESCR64(d, qd_pa, conf.rxRingBasePA[j]);
            size = VMXNET3_READ_RX_QUEUE_DESCR32(d, qd_pa, conf.rxRingSize[j]);
            vmxnet3_ring_init(d, &s->rxq_descr[i].rx_ring[j], pa, size,
                              sizeof(struct Vmxnet3_RxDesc), false);
            VMW_CFPRN("RX queue %d:%d: Base: %" PRIx64 ", Size: %d",
                      i, j, pa, size);
        }

         
        pa = VMXNET3_READ_RX_QUEUE_DESCR64(d, qd_pa, conf.compRingBasePA);
        size = VMXNET3_READ_RX_QUEUE_DESCR32(d, qd_pa, conf.compRingSize);
        vmxnet3_ring_init(d, &s->rxq_descr[i].comp_ring, pa, size,
                          sizeof(struct Vmxnet3_RxCompDesc), true);
        VMW_CFPRN("RXC queue %d: Base: %" PRIx64 ", Size: %d", i, pa, size);

        s->rxq_descr[i].rx_stats_pa =
            qd_pa + offsetof(struct Vmxnet3_RxQueueDesc, stats);
        memset(&s->rxq_descr[i].rxq_stats, 0,
               sizeof(s->rxq_descr[i].rxq_stats));
    }

    vmxnet3_validate_interrupts(s);

     
    smp_wmb();

    vmxnet3_reset_mac(s);

    s->device_active = true;
}