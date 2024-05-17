bt_status_t btif_hh_execute_service(BOOLEAN b_enable)
{
 if (b_enable)
 {
  
          BTA_HhEnable(BTUI_HH_SECURITY, bte_hh_evt);
 }
 else {
  
         BTA_HhDisable();
 }
 return BT_STATUS_SUCCESS;
}
