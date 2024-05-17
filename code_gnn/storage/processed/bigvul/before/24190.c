ar6000_hci_event_rcv_evt(struct ar6_softc *ar, WMI_HCI_EVENT *cmd)
{
    void *osbuf = NULL;
    s8 i;
    u8 size, *buf;
    int ret = 0;

    size = cmd->evt_buf_sz + 4;
    osbuf = A_NETBUF_ALLOC(size);
    if (osbuf == NULL) {
       ret = A_NO_MEMORY;
       A_PRINTF("Error in allocating netbuf \n");
       return;
    }

    A_NETBUF_PUT(osbuf, size);
    buf = (u8 *)A_NETBUF_DATA(osbuf);
     
    *((short *)buf) = WMI_HCI_EVENT_EVENTID;
    buf += sizeof(int);
    memcpy(buf, cmd->buf, cmd->evt_buf_sz);

    ar6000_deliver_frames_to_nw_stack(ar->arNetDev, osbuf);
    if(loghci) {
        A_PRINTF_LOG("HCI Event From PAL <-- \n");
        for(i = 0; i < cmd->evt_buf_sz; i++) {
           A_PRINTF_LOG("0x%02x ", cmd->buf[i]);
           if((i % 10) == 0) {
               A_PRINTF_LOG("\n");
           }
        }
        A_PRINTF_LOG("\n");
        A_PRINTF_LOG("==================================\n");
    }
}