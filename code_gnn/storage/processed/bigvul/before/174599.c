void btm_sec_clr_temp_auth_service (BD_ADDR bda)
{
    tBTM_SEC_DEV_REC   *p_dev_rec;

 if ((p_dev_rec = btm_find_dev (bda)) == NULL)
 {
        BTM_TRACE_WARNING ("btm_sec_clr_temp_auth_service() - no dev CB");
 return;
 }

  
 if (p_dev_rec->last_author_service_id != BTM_SEC_NO_LAST_SERVICE_ID && p_dev_rec->p_cur_service)
 {
        BTM_TRACE_DEBUG ("btm_sec_clr_auth_service_by_psm [clearing device: %02x:%02x:%02x:%02x:%02x:%02x]",
                    bda[0], bda[1], bda[2], bda[3], bda[4], bda[5]);

        p_dev_rec->last_author_service_id = BTM_SEC_NO_LAST_SERVICE_ID;
 }
}
