uint16_t btif_dm_get_connection_state(const bt_bdaddr_t *bd_addr)
{
 uint8_t *bda = (uint8_t*)bd_addr->address;
 uint16_t rc = BTA_DmGetConnectionState(bda);

 if (rc != 0)
 {
 uint8_t flags = 0;

        BTM_GetSecurityFlagsByTransport(bda, &flags, BT_TRANSPORT_BR_EDR);
        BTIF_TRACE_DEBUG("%s: security flags (BR/EDR)=0x%02x", __FUNCTION__, flags);
 if (flags & BTM_SEC_FLAG_ENCRYPTED)
            rc |= ENCRYPTED_BREDR;

        BTM_GetSecurityFlagsByTransport(bda, &flags, BT_TRANSPORT_LE);
        BTIF_TRACE_DEBUG("%s: security flags (LE)=0x%02x", __FUNCTION__, flags);
 if (flags & BTM_SEC_FLAG_ENCRYPTED)
            rc |= ENCRYPTED_LE;
 }

 return rc;
}
