static bt_status_t btpan_enable(int local_role)
{
#if BTA_PAN_INCLUDED == TRUE
    BTIF_TRACE_DEBUG("%s - local_role: %d", __func__, local_role);
 int bta_pan_role = btpan_role_to_bta(local_role);
    BTA_PanSetRole(bta_pan_role, &bta_panu_info, NULL, &bta_pan_nap_info);
    btpan_dev_local_role = local_role;
 return BT_STATUS_SUCCESS;
#else
 return BT_STATUS_FAIL;
#endif
}
