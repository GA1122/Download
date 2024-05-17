void btif_enable_bluetooth_evt(tBTA_STATUS status)
{
 const controller_t *controller = controller_get_interface();
 bdstr_t bdstr;
    bdaddr_to_string(controller->get_address(), bdstr, sizeof(bdstr));

    BTIF_TRACE_DEBUG("%s: status %d, local bd [%s]", __FUNCTION__, status, bdstr);

 if (bdcmp(btif_local_bd_addr.address, controller->get_address()->address))
 {
 bdstr_t old_address;
 bt_property_t prop;

        bdaddr_to_string(&btif_local_bd_addr, old_address, sizeof(old_address));

  
        BTIF_TRACE_WARNING("***********************************************");
        BTIF_TRACE_WARNING("BTIF init BDA was %s", old_address);
        BTIF_TRACE_WARNING("Controller BDA is %s", bdstr);
        BTIF_TRACE_WARNING("***********************************************");

        btif_local_bd_addr = *controller->get_address();

        btif_config_set_str("Adapter", "Address", bdstr);
        btif_config_save();

        prop.type = BT_PROPERTY_BDADDR;
        prop.val = (void*)&btif_local_bd_addr;
        prop.len = sizeof(bt_bdaddr_t);
        HAL_CBACK(bt_hal_cbacks, adapter_properties_cb, BT_STATUS_SUCCESS, 1, &prop);
 }

    bte_main_postload_cfg();
#if (defined(HCILP_INCLUDED) && HCILP_INCLUDED == TRUE)
    bte_main_enable_lpm(TRUE);
#endif
  

  
 if (status == BTA_SUCCESS)
 {
  
        btif_sock_init();

  
        btif_pan_init();

  
        bte_load_did_conf(BTE_DID_CONF_FILE);

#ifdef BTIF_DM_OOB_TEST
        btif_dm_load_local_oob();
#endif

        future_ready(stack_manager_get_hack_future(), FUTURE_SUCCESS);
 }
 else
 {
  
        btif_sock_cleanup();

        btif_pan_cleanup();

        future_ready(stack_manager_get_hack_future(), FUTURE_FAIL);
 }
}
