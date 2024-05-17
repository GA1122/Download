void ar6000_dtimexpiry_event(struct ar6_softc *ar)
{
    bool isMcastQueued = false;
    struct sk_buff *skb = NULL;

     
    if (ar->sta_list_index == 0) {
        return;
    }

    A_MUTEX_LOCK(&ar->mcastpsqLock);
    isMcastQueued = A_NETBUF_QUEUE_EMPTY(&ar->mcastpsq);
    A_MUTEX_UNLOCK(&ar->mcastpsqLock);

    A_ASSERT(isMcastQueued == false);

     
     
    ar->DTIMExpired = true;

    A_MUTEX_LOCK(&ar->mcastpsqLock);
    while (!A_NETBUF_QUEUE_EMPTY(&ar->mcastpsq)) {
        skb = A_NETBUF_DEQUEUE(&ar->mcastpsq);
        A_MUTEX_UNLOCK(&ar->mcastpsqLock);

        ar6000_data_tx(skb, ar->arNetDev);

        A_MUTEX_LOCK(&ar->mcastpsqLock);
    }
    A_MUTEX_UNLOCK(&ar->mcastpsqLock);

     
    ar->DTIMExpired = false;

     
    wmi_set_pvb_cmd(ar->arWmi, MCAST_AID, 0);
}
