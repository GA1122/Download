bool btif_get_device_type(const BD_ADDR bd_addr, int *p_device_type)
{
 if (p_device_type == NULL)
 return FALSE;

 bt_bdaddr_t bda;
    bdcpy(bda.address, bd_addr);

 bdstr_t bd_addr_str;
    bdaddr_to_string(&bda, bd_addr_str, sizeof(bd_addr_str));

 if (!btif_config_get_int(bd_addr_str, "DevType", p_device_type))
 return FALSE;

    LOG_DEBUG("%s: Device [%s] type %d", __FUNCTION__, bd_addr_str, *p_device_type);
 return TRUE;
}
