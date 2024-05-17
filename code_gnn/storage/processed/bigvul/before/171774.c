void btif_dm_proc_loc_oob(BOOLEAN valid, BT_OCTET16 c, BT_OCTET16 r)
{
 FILE *fp;
 char *path_a = "/data/misc/bluedroid/LOCAL/a.key";
 char *path_b = "/data/misc/bluedroid/LOCAL/b.key";
 char *path = NULL;
 char prop_oob[PROPERTY_VALUE_MAX];
    BTIF_TRACE_DEBUG("btif_dm_proc_loc_oob: valid=%d", valid);
 if (oob_cb.sp_c[0] == 0 && oob_cb.sp_c[1] == 0 &&
        oob_cb.sp_c[2] == 0 && oob_cb.sp_c[3] == 0 &&
        valid)
 {
        BTIF_TRACE_DEBUG("save local OOB data in memory");
        memcpy(oob_cb.sp_c, c, BT_OCTET16_LEN);
        memcpy(oob_cb.sp_r, r, BT_OCTET16_LEN);
        property_get("service.brcm.bt.oob", prop_oob, "3");
        BTIF_TRACE_DEBUG("btif_dm_proc_loc_oob prop_oob = %s",prop_oob);
 if (prop_oob[0] == '1')
            path = path_a;
 else if (prop_oob[0] == '2')
            path = path_b;
 if (path)
 {
            fp = fopen(path, "wb+");
 if (fp == NULL)
 {
                BTIF_TRACE_DEBUG("btif_dm_proc_loc_oob: failed to save local OOB data to %s", path);
 }
 else
 {
                BTIF_TRACE_DEBUG("btif_dm_proc_loc_oob: save local OOB data into file %s",path);
                fwrite (c , 1 , BT_OCTET16_LEN , fp );
                fwrite (r , 1 , BT_OCTET16_LEN , fp );
                fclose(fp);
 }
 }
 }
}
