tBTM_STATUS BTM_SetEncryption (BD_ADDR bd_addr, tBT_TRANSPORT transport, tBTM_SEC_CBACK *p_callback,
 void *p_ref_data)
{
    tBTM_SEC_DEV_REC  *p_dev_rec;
    tBTM_STATUS       rc;
#if BLE_INCLUDED == TRUE
   tACL_CONN         *p = btm_bda_to_acl(bd_addr, transport);
#endif

    p_dev_rec = btm_find_dev (bd_addr);

 if (!p_dev_rec ||
 (transport == BT_TRANSPORT_BR_EDR && p_dev_rec->hci_handle == BTM_SEC_INVALID_HANDLE)
#if BLE_INCLUDED == TRUE
 || (transport == BT_TRANSPORT_LE && p_dev_rec->ble_hci_handle == BTM_SEC_INVALID_HANDLE)
#endif
 )
 {
  
        BTM_TRACE_WARNING ("Security Manager: BTM_SetEncryption not connected");

 if (p_callback)
 (*p_callback) (bd_addr, transport, p_ref_data, BTM_WRONG_MODE);

 return(BTM_WRONG_MODE);
 }

 if ((transport == BT_TRANSPORT_BR_EDR &&
 (p_dev_rec->sec_flags &  BTM_SEC_ENCRYPTED))
#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE
 || (transport == BT_TRANSPORT_LE &&
 (p_dev_rec->sec_flags & BTM_SEC_LE_ENCRYPTED))
#endif
 )
 {
        BTM_TRACE_EVENT ("Security Manager: BTM_SetEncryption already encrypted");

 if (p_callback)
 (*p_callback) (bd_addr, transport, p_ref_data, BTM_SUCCESS);

 return(BTM_SUCCESS);
 }

 if (p_dev_rec->p_callback)
 {
  
        BTM_TRACE_WARNING ("Security Manager: BTM_SetEncryption busy");

 if (p_callback)
 (*p_callback) (bd_addr, transport, p_ref_data, BTM_BUSY);

 return(BTM_BUSY);
 }

    p_dev_rec->p_callback        = p_callback;
    p_dev_rec->p_ref_data        = p_ref_data;
    p_dev_rec->security_required |= (BTM_SEC_IN_AUTHENTICATE | BTM_SEC_IN_ENCRYPT);
    p_dev_rec->is_originator     = FALSE;

    BTM_TRACE_API ("Security Manager: BTM_SetEncryption Handle:%d State:%d Flags:0x%x Required:0x%x",
                    p_dev_rec->hci_handle, p_dev_rec->sec_state, p_dev_rec->sec_flags,
                    p_dev_rec->security_required);

#if BLE_INCLUDED == TRUE && SMP_INCLUDED == TRUE
 if (transport == BT_TRANSPORT_LE)
 {
        rc = btm_ble_set_encryption(bd_addr, p_ref_data, p->link_role);
 }
 else
#endif

        rc = btm_sec_execute_procedure (p_dev_rec);

 if (rc != BTM_CMD_STARTED && rc != BTM_BUSY)
 {
 if (p_callback)
 {
            p_dev_rec->p_callback = NULL;
 (*p_callback) (bd_addr, transport, p_dev_rec->p_ref_data, rc);
 }
 }
 return(rc);
}
