bt_status_t btif_dut_mode_configure(uint8_t enable)
{
    BTIF_TRACE_DEBUG("%s", __FUNCTION__);

 if (!stack_manager_get_interface()->get_stack_is_running()) {
        BTIF_TRACE_ERROR("btif_dut_mode_configure : Bluetooth not enabled");
 return BT_STATUS_NOT_READY;
 }

    btif_dut_mode = enable;
 if (enable == 1) {
        BTA_EnableTestMode();
 } else {
        BTA_DisableTestMode();
 }
 return BT_STATUS_SUCCESS;
}
