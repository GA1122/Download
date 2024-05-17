 static void btif_fetch_local_bdaddr(bt_bdaddr_t *local_addr)
 {
  char val[PROPERTY_VALUE_MAX] = {0};
  uint8_t valid_bda = FALSE;
  int val_size = 0;
  const uint8_t null_bdaddr[BD_ADDR_LEN] = {0,0,0,0,0,0};
 
   
  if (property_get(PROPERTY_BT_BDADDR_PATH, val, NULL))
  {
  int addr_fd;
 
  
          BTIF_TRACE_DEBUG("%s, local bdaddr is stored in %s", __func__, val);
  
        if ((addr_fd = open(val, O_RDONLY)) != -1)
//         if ((addr_fd = TEMP_FAILURE_RETRY(open(val, O_RDONLY))) != -1)
          {
              memset(val, 0, sizeof(val));
            read(addr_fd, val, FACTORY_BT_BDADDR_STORAGE_LEN);
//             TEMP_FAILURE_RETRY(read(addr_fd, val, FACTORY_BT_BDADDR_STORAGE_LEN));
               
              if ((string_to_bdaddr(val, local_addr)) &&
                  (memcmp(local_addr->address, null_bdaddr, BD_ADDR_LEN) != 0))
  {
                 valid_bda = TRUE;
                 BTIF_TRACE_DEBUG("%s: Got Factory BDA %s", __func__, val);
  }
             close(addr_fd);
  }
  }
 
  if(!valid_bda)
  {
         val_size = sizeof(val);
  if(btif_config_get_str("Adapter", "Address", val, &val_size))
  {
             string_to_bdaddr(val, local_addr);
             BTIF_TRACE_DEBUG("local bdaddr from bt_config.xml is  %s", val);
  return;
  }
  }
 
   
  if (!valid_bda) {
         valid_bda = btif_fetch_property(PERSIST_BDADDR_PROPERTY, local_addr);
  }
 
   
  if (!valid_bda) {
         valid_bda = btif_fetch_property(FACTORY_BT_ADDR_PROPERTY, local_addr);
  }
 
   
  if (!valid_bda)
  {
  bdstr_t bdstr;
   
         srand((unsigned int) (time(0)));
 
   
         local_addr->address[0] = 0x22;
         local_addr->address[1] = 0x22;
         local_addr->address[2] = (uint8_t) ((rand() >> 8) & 0xFF);
         local_addr->address[3] = (uint8_t) ((rand() >> 8) & 0xFF);
         local_addr->address[4] = (uint8_t) ((rand() >> 8) & 0xFF);
         local_addr->address[5] = (uint8_t) ((rand() >> 8) & 0xFF);
 
   
         bdaddr_to_string(local_addr, bdstr, sizeof(bdstr));
 
         BTIF_TRACE_DEBUG("No preset BDA. Generating BDA: %s for prop %s",
  (char*)bdstr, PERSIST_BDADDR_PROPERTY);
 
  if (property_set(PERSIST_BDADDR_PROPERTY, (char*)bdstr) < 0)
             BTIF_TRACE_ERROR("Failed to set random BDA in prop %s",PERSIST_BDADDR_PROPERTY);
  }
 
  bdstr_t bdstr;
     bdaddr_to_string(local_addr, bdstr, sizeof(bdstr));
     val_size = sizeof(val);
  if (btif_config_get_str("Adapter", "Address", val, &val_size))
  {
  if (strcmp(bdstr, val) ==0)
  {
  return;
  }
  }
     btif_config_set_str("Adapter", "Address", bdstr);
 }