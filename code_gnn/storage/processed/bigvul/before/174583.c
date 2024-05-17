static char *btm_pair_state_descr (tBTM_PAIRING_STATE state)
{
#if (BT_TRACE_VERBOSE == TRUE)
 switch (state)
 {
 case BTM_PAIR_STATE_IDLE: return("IDLE");
 case BTM_PAIR_STATE_GET_REM_NAME: return("GET_REM_NAME");
 case BTM_PAIR_STATE_WAIT_PIN_REQ: return("WAIT_PIN_REQ");
 case BTM_PAIR_STATE_WAIT_LOCAL_PIN: return("WAIT_LOCAL_PIN");
 case BTM_PAIR_STATE_WAIT_NUMERIC_CONFIRM: return("WAIT_NUM_CONFIRM");
 case BTM_PAIR_STATE_KEY_ENTRY: return("KEY_ENTRY");
 case BTM_PAIR_STATE_WAIT_LOCAL_OOB_RSP: return("WAIT_LOCAL_OOB_RSP");
 case BTM_PAIR_STATE_WAIT_LOCAL_IOCAPS: return("WAIT_LOCAL_IOCAPS");
 case BTM_PAIR_STATE_INCOMING_SSP: return("INCOMING_SSP");
 case BTM_PAIR_STATE_WAIT_AUTH_COMPLETE: return("WAIT_AUTH_COMPLETE");
 case BTM_PAIR_STATE_WAIT_DISCONNECT: return("WAIT_DISCONNECT");
 }

 return("???");
#else
    sprintf(btm_cb.state_temp_buffer,"%hu",state);

 return(btm_cb.state_temp_buffer);
#endif
}
