BOOLEAN btif_dm_proc_rmt_oob(BD_ADDR bd_addr,  BT_OCTET16 p_c, BT_OCTET16 p_r)
{
 char t[128];
 FILE *fp;
 char *path_a = "/data/misc/bluedroid/LOCAL/a.key";
 char *path_b = "/data/misc/bluedroid/LOCAL/b.key";
 char *path = NULL;
 char prop_oob[PROPERTY_VALUE_MAX];
    BOOLEAN result = FALSE;
 bt_bdaddr_t bt_bd_addr;
    bdcpy(oob_cb.oob_bdaddr, bd_addr);
    property_get("service.brcm.bt.oob", prop_oob, "3");
    BTIF_TRACE_DEBUG("btif_dm_proc_rmt_oob prop_oob = %s",prop_oob);
 if (prop_oob[0] == '1')
        path = path_b;
 else if (prop_oob[0] == '2')
        path = path_a;
 if (path)
 {
        fp = fopen(path, "rb");
 if (fp == NULL)
 {
            BTIF_TRACE_DEBUG("btapp_dm_rmt_oob_reply: failed to read OOB keys from %s",path);
 return FALSE;
 }
 else
 {
            BTIF_TRACE_DEBUG("btif_dm_proc_rmt_oob: read OOB data from %s",path);
            fread (p_c , 1 , BT_OCTET16_LEN , fp );
            fread (p_r , 1 , BT_OCTET16_LEN , fp );
            fclose(fp);
 }
        BTIF_TRACE_DEBUG("----btif_dm_proc_rmt_oob: TRUE");
        sprintf(t, "%02x:%02x:%02x:%02x:%02x:%02x",
                oob_cb.oob_bdaddr[0], oob_cb.oob_bdaddr[1], oob_cb.oob_bdaddr[2],
                oob_cb.oob_bdaddr[3], oob_cb.oob_bdaddr[4], oob_cb.oob_bdaddr[5]);
        BTIF_TRACE_DEBUG("----btif_dm_proc_rmt_oob: peer_bdaddr = %s", t);
        sprintf(t, "%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x",
                p_c[0], p_c[1], p_c[2],  p_c[3],  p_c[4],  p_c[5],  p_c[6],  p_c[7],
                p_c[8], p_c[9], p_c[10], p_c[11], p_c[12], p_c[13], p_c[14], p_c[15]);
        BTIF_TRACE_DEBUG("----btif_dm_proc_rmt_oob: c = %s",t);
        sprintf(t, "%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x",
                p_r[0], p_r[1], p_r[2],  p_r[3],  p_r[4],  p_r[5],  p_r[6],  p_r[7],
                p_r[8], p_r[9], p_r[10], p_r[11], p_r[12], p_r[13], p_r[14], p_r[15]);
        BTIF_TRACE_DEBUG("----btif_dm_proc_rmt_oob: r = %s",t);
        bdcpy(bt_bd_addr.address, bd_addr);
        btif_transfer_context(btif_dm_generic_evt, BTIF_DM_CB_BOND_STATE_BONDING,
 (char *)&bt_bd_addr, sizeof(bt_bdaddr_t), NULL);
        result = TRUE;
 }
    BTIF_TRACE_DEBUG("btif_dm_proc_rmt_oob result=%d",result);
 return result;
}
