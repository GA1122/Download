void btif_disable_bluetooth_evt(void)
{
    BTIF_TRACE_DEBUG("%s", __FUNCTION__);

#if (defined(HCILP_INCLUDED) && HCILP_INCLUDED == TRUE)
    bte_main_enable_lpm(FALSE);
#endif

#if (BLE_INCLUDED == TRUE)
     BTA_VendorCleanup();
#endif

     bte_main_disable();

  
    future_ready(stack_manager_get_hack_future(), FUTURE_SUCCESS);
}
