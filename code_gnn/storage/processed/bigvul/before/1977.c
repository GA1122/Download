SPICE_GNUC_VISIBLE int spice_server_set_compat_version(SpiceServer *s,
                                                       spice_compat_version_t version)
{
    if (version < SPICE_COMPAT_VERSION_0_6) {
         
        return -1;
    }

    if (version > SPICE_COMPAT_VERSION_CURRENT) {
         
        return -1;
    }
    return 0;
}
