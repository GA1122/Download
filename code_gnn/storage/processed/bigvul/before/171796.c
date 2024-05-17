static BOOLEAN check_eir_remote_name(tBTA_DM_SEARCH *p_search_data,
                            UINT8 *p_remote_name, UINT8 *p_remote_name_len)
{
    UINT8 *p_eir_remote_name = NULL;
    UINT8 remote_name_len = 0;

  
 if (p_search_data->inq_res.p_eir)
 {
        p_eir_remote_name = BTM_CheckEirData(p_search_data->inq_res.p_eir,
                BTM_EIR_COMPLETE_LOCAL_NAME_TYPE, &remote_name_len);
 if (!p_eir_remote_name)
 {
            p_eir_remote_name = BTM_CheckEirData(p_search_data->inq_res.p_eir,
                    BTM_EIR_SHORTENED_LOCAL_NAME_TYPE, &remote_name_len);
 }

 if (p_eir_remote_name)
 {
 if (remote_name_len > BD_NAME_LEN)
                remote_name_len = BD_NAME_LEN;

 if (p_remote_name && p_remote_name_len)
 {
                memcpy(p_remote_name, p_eir_remote_name, remote_name_len);
 *(p_remote_name + remote_name_len) = 0;
 *p_remote_name_len = remote_name_len;
 }

 return TRUE;
 }
 }

 return FALSE;

}
