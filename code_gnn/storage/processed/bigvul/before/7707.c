static void pdu_push_and_notify(V9fsPDU *pdu)
{
    virtio_9p_push_and_notify(pdu);
}
