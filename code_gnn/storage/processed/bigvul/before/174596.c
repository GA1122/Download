static void btm_sec_check_upgrade(tBTM_SEC_DEV_REC  *p_dev_rec, BOOLEAN is_originator)
{
    tBTM_SP_UPGRADE     evt_data;

    BTM_TRACE_DEBUG ("btm_sec_check_upgrade...");

  
 if (!(p_dev_rec->sec_flags & BTM_SEC_LINK_KEY_KNOWN))
 return;
 if (btm_sec_is_upgrade_possible (p_dev_rec, is_originator) == TRUE)
 {
        BTM_TRACE_DEBUG ("need upgrade!! sec_flags:0x%x", p_dev_rec->sec_flags);
  
        memcpy (evt_data.bd_addr, p_dev_rec->bd_addr, BD_ADDR_LEN);
        evt_data.upgrade = TRUE;
 if (btm_cb.api.p_sp_callback)
 (*btm_cb.api.p_sp_callback) (BTM_SP_UPGRADE_EVT, (tBTM_SP_EVT_DATA *)&evt_data);

        BTM_TRACE_DEBUG ("evt_data.upgrade:0x%x", evt_data.upgrade);
 if (evt_data.upgrade)
 {
  
            p_dev_rec->sm4 |= BTM_SM4_UPGRADE;

  
            p_dev_rec->sec_flags &= ~(BTM_SEC_LINK_KEY_KNOWN | BTM_SEC_LINK_KEY_AUTHED);
            p_dev_rec->sec_flags &= ~BTM_SEC_AUTHENTICATED;
            BTM_TRACE_DEBUG ("sec_flags:0x%x", p_dev_rec->sec_flags);
 }
 }
}
