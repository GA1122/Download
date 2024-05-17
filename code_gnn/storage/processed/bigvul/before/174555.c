tBTM_STATUS BTM_SecBond (BD_ADDR bd_addr, UINT8 pin_len, UINT8 *p_pin, UINT32 trusted_mask[])
{
    tBT_TRANSPORT   transport = BT_TRANSPORT_BR_EDR;
#if BLE_INCLUDED == TRUE
 if (BTM_UseLeLink(bd_addr))
        transport = BT_TRANSPORT_LE;
#endif
 return btm_sec_bond_by_transport(bd_addr, transport, pin_len, p_pin, trusted_mask);
}
