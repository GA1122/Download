void btif_remote_properties_evt(bt_status_t status, bt_bdaddr_t *remote_addr,
 uint32_t num_props, bt_property_t *p_props)
{
    HAL_CBACK(bt_hal_cbacks, remote_device_properties_cb,
                     status, remote_addr, num_props, p_props);
}
