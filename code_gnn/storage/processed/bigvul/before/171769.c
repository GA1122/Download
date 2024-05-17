void btif_dm_load_local_oob(void)
{
 char prop_oob[PROPERTY_VALUE_MAX];
    property_get("service.brcm.bt.oob", prop_oob, "3");
    BTIF_TRACE_DEBUG("btif_dm_load_local_oob prop_oob = %s",prop_oob);
 if (prop_oob[0] != '3')
 {
#if (BTM_OOB_INCLUDED == TRUE)
 if (oob_cb.sp_c[0] == 0 && oob_cb.sp_c[1] == 0 &&
            oob_cb.sp_c[2] == 0 && oob_cb.sp_c[3] == 0 )
 {
            BTIF_TRACE_DEBUG("btif_dm_load_local_oob: read OOB, call BTA_DmLocalOob()");
            BTA_DmLocalOob();
 }
#else
        BTIF_TRACE_ERROR("BTM_OOB_INCLUDED is FALSE!!(btif_dm_load_local_oob)");
#endif
 }
}
