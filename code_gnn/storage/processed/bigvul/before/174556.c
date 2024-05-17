tBTM_STATUS BTM_SecBondByTransport (BD_ADDR bd_addr, tBT_TRANSPORT transport,
                                    UINT8 pin_len, UINT8 *p_pin, UINT32 trusted_mask[])
{
#if SMP_INCLUDED == TRUE
    tBT_DEVICE_TYPE     dev_type;
    tBLE_ADDR_TYPE      addr_type;

    BTM_ReadDevInfo(bd_addr, &dev_type, &addr_type);
  
 if ((transport == BT_TRANSPORT_LE && (dev_type & BT_DEVICE_TYPE_BLE) == 0) ||
 (transport == BT_TRANSPORT_BR_EDR && (dev_type & BT_DEVICE_TYPE_BREDR) == 0))
 {
 return BTM_ILLEGAL_ACTION;
 }
#endif
 return btm_sec_bond_by_transport(bd_addr, transport, pin_len, p_pin, trusted_mask);
}
