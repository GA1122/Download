void btm_create_conn_cancel_complete (UINT8 *p)
{
    UINT8       status;

    STREAM_TO_UINT8 (status, p);
    BTM_TRACE_EVENT ("btm_create_conn_cancel_complete(): in State: %s  status:%d",
                      btm_pair_state_descr(btm_cb.pairing_state), status);

  
 switch (status)
 {
 case HCI_SUCCESS:
            btm_sec_bond_cancel_complete();
 break;
 case HCI_ERR_CONNECTION_EXISTS:
 case HCI_ERR_NO_CONNECTION:
 default:
  
 if (btm_cb.api.p_bond_cancel_cmpl_callback)
                btm_cb.api.p_bond_cancel_cmpl_callback(BTM_ERR_PROCESSING);
 break;
 }
}
