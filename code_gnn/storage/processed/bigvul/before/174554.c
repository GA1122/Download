BOOLEAN  BTM_SecAddRmtNameNotifyCallback (tBTM_RMT_NAME_CALLBACK *p_callback)
{
 int i;

 for (i = 0; i < BTM_SEC_MAX_RMT_NAME_CALLBACKS; i++)
 {
 if (btm_cb.p_rmt_name_callback[i] == NULL)
 {
            btm_cb.p_rmt_name_callback[i] = p_callback;
 return(TRUE);
 }
 }

 return(FALSE);
}
