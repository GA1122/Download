static void btm_send_link_key_notif (tBTM_SEC_DEV_REC *p_dev_rec)
{
 if (btm_cb.api.p_link_key_callback)
 (*btm_cb.api.p_link_key_callback) (p_dev_rec->bd_addr, p_dev_rec->dev_class,
                                           p_dev_rec->sec_bd_name, p_dev_rec->link_key,
                                           p_dev_rec->link_key_type);
}
