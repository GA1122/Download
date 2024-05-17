bt_status_t btif_dm_cancel_discovery(void)
{
    BTIF_TRACE_EVENT("%s", __FUNCTION__);
    BTA_DmSearchCancel();
 return BT_STATUS_SUCCESS;
}
