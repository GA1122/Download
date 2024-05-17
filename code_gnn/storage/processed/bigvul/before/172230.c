static void uipc_flush_locked(tUIPC_CH_ID ch_id)
{
 if (ch_id >= UIPC_CH_NUM)
 return;

 switch(ch_id)
 {
 case UIPC_CH_ID_AV_CTRL:
            uipc_flush_ch_locked(UIPC_CH_ID_AV_CTRL);
 break;

 case UIPC_CH_ID_AV_AUDIO:
            uipc_flush_ch_locked(UIPC_CH_ID_AV_AUDIO);
 break;
 }
}
