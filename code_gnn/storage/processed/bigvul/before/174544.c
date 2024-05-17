void BTM_DeviceAuthorized (BD_ADDR bd_addr, UINT8 res, UINT32 trusted_mask[])
{
    tBTM_SEC_DEV_REC *p_dev_rec;

 if ((p_dev_rec = btm_find_dev (bd_addr)) == NULL)
 {
        BTM_TRACE_WARNING ("Security Manager: Attempting Authorization of Unknown Device Address [%02x%02x%02x%02x%02x%02x]",
                            bd_addr[0], bd_addr[1], bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5]);
 return;
 }

    BTM_TRACE_EVENT ("Security Manager: authorized status:%d State:%d Trusted:%08x %08x",
                      res, (p_dev_rec) ? p_dev_rec->sec_state : 0, trusted_mask[0], trusted_mask[1]);

 if (res == BTM_SUCCESS)
 {
        p_dev_rec->sec_flags   |= BTM_SEC_AUTHORIZED;
 if (trusted_mask)
 {
            BTM_SEC_COPY_TRUSTED_DEVICE(trusted_mask, p_dev_rec->trusted_mask);
 }

  
 if (!p_dev_rec->is_originator)
 {
            BTM_TRACE_DEBUG("BTM_DeviceAuthorized: Setting last_author_service_id to %d",
                             p_dev_rec->p_cur_service->service_id);
            p_dev_rec->last_author_service_id = p_dev_rec->p_cur_service->service_id;
 }
 }

 if (p_dev_rec->sec_state != BTM_SEC_STATE_AUTHORIZING)
 return;

    p_dev_rec->sec_state = BTM_SEC_STATE_IDLE;

 if (res != BTM_SUCCESS)
 {
        btm_sec_dev_rec_cback_event (p_dev_rec, res, FALSE);
 return;
 }

 if ((res = (UINT8)btm_sec_execute_procedure (p_dev_rec)) != BTM_CMD_STARTED)
 {
        btm_sec_dev_rec_cback_event (p_dev_rec, res, FALSE);
 }
}
