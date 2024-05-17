void btif_adapter_properties_evt(bt_status_t status, uint32_t num_props,
 bt_property_t *p_props)
{
    HAL_CBACK(bt_hal_cbacks, adapter_properties_cb,
                     status, num_props, p_props);

}
