static void btif_update_remote_version_property(bt_bdaddr_t *p_bd)
{
 bt_property_t property;
    UINT8 lmp_ver = 0;
    UINT16 lmp_subver = 0;
    UINT16 mfct_set = 0;
    tBTM_STATUS btm_status;
 bt_remote_version_t info;
 bt_status_t status;
 bdstr_t bdstr;

    btm_status = BTM_ReadRemoteVersion(*(BD_ADDR*)p_bd, &lmp_ver,
 &mfct_set, &lmp_subver);

    LOG_DEBUG("remote version info [%s]: %x, %x, %x", bdaddr_to_string(p_bd, bdstr, sizeof(bdstr)),
               lmp_ver, mfct_set, lmp_subver);

 if (btm_status == BTM_SUCCESS)
 {
        info.manufacturer = mfct_set;
        info.sub_ver = lmp_subver;
        info.version = lmp_ver;
        BTIF_STORAGE_FILL_PROPERTY(&property,
                            BT_PROPERTY_REMOTE_VERSION_INFO, sizeof(bt_remote_version_t),
 &info);
        status = btif_storage_set_remote_device_property(p_bd, &property);
        ASSERTC(status == BT_STATUS_SUCCESS, "failed to save remote version", status);
 }
}
