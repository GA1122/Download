void btm_sec_mkey_comp_event (UINT16 handle, UINT8 status, UINT8 key_flg)
{
    tBTM_SEC_DEV_REC  *p_dev_rec = btm_find_dev_by_handle (handle);
    UINT8 bd_addr[BD_ADDR_LEN] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff} ;

    BTM_TRACE_EVENT ("Security Manager: mkey comp status:%d State:%d",
                      status, (p_dev_rec) ? p_dev_rec->sec_state : 0);

  
  
 if (btm_cb.mkey_cback)
 {
 if (!p_dev_rec)
 (btm_cb.mkey_cback)(bd_addr, status, key_flg );
 else
 (btm_cb.mkey_cback)(p_dev_rec->bd_addr, status, key_flg );
 }
}
