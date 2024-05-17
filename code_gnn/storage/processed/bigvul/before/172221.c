extern BOOLEAN UIPC_Ioctl(tUIPC_CH_ID ch_id, UINT32 request, void *param)
{
    BTIF_TRACE_DEBUG("#### UIPC_Ioctl : ch_id %d, request %d ####", ch_id, request);

    UIPC_LOCK();

 switch(request)
 {
 case UIPC_REQ_RX_FLUSH:
            uipc_flush_locked(ch_id);
 break;

 case UIPC_REG_CBACK:
            uipc_main.ch[ch_id].cback = (tUIPC_RCV_CBACK*)param;
 break;

 case UIPC_REG_REMOVE_ACTIVE_READSET:

  
 if (uipc_main.ch[ch_id].fd != UIPC_DISCONNECTED)
 {
  
                FD_CLR(uipc_main.ch[ch_id].fd, &uipc_main.active_set);

  
                uipc_wakeup_locked();
 }
 break;

 case UIPC_SET_READ_POLL_TMO:
            uipc_main.ch[ch_id].read_poll_tmo_ms = (intptr_t)param;
            BTIF_TRACE_EVENT("UIPC_SET_READ_POLL_TMO : CH %d, TMO %d ms", ch_id, uipc_main.ch[ch_id].read_poll_tmo_ms );
 break;

 default:
            BTIF_TRACE_EVENT("UIPC_Ioctl : request not handled (%d)", request);
 break;
 }

    UIPC_UNLOCK();

 return FALSE;
}
