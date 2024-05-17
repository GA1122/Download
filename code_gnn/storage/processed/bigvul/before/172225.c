const char* dump_uipc_event(tUIPC_EVENT event)
{
 switch(event)
 {
        CASE_RETURN_STR(UIPC_OPEN_EVT)
        CASE_RETURN_STR(UIPC_CLOSE_EVT)
        CASE_RETURN_STR(UIPC_RX_DATA_EVT)
        CASE_RETURN_STR(UIPC_RX_DATA_READY_EVT)
        CASE_RETURN_STR(UIPC_TX_DATA_READY_EVT)
 default:
 return "UNKNOWN MSG ID";
 }
}
