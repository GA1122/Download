vmxnet3_send_packet(VMXNET3State *s, uint32_t qidx)
{
    Vmxnet3PktStatus status = VMXNET3_PKT_STATUS_OK;

    if (!vmxnet3_setup_tx_offloads(s)) {
        status = VMXNET3_PKT_STATUS_ERROR;
        goto func_exit;
    }

     
    vmxnet3_dump_virt_hdr(vmxnet_tx_pkt_get_vhdr(s->tx_pkt));
    vmxnet_tx_pkt_dump(s->tx_pkt);

    if (!vmxnet_tx_pkt_send(s->tx_pkt, qemu_get_queue(s->nic))) {
        status = VMXNET3_PKT_STATUS_DISCARD;
        goto func_exit;
    }

func_exit:
    vmxnet3_on_tx_done_update_stats(s, qidx, status);
    return (status == VMXNET3_PKT_STATUS_OK);
}
