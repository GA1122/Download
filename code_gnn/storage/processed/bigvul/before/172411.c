omx_venc::omx_venc()
{
#ifdef _ANDROID_ICS_
    meta_mode_enable = false;
    memset(meta_buffer_hdr,0,sizeof(meta_buffer_hdr));
    memset(meta_buffers,0,sizeof(meta_buffers));
    memset(opaque_buffer_hdr,0,sizeof(opaque_buffer_hdr));
    mUseProxyColorFormat = false;
    get_syntaxhdr_enable = false;
#endif
    bframes = entropy = 0;
 char property_value[PROPERTY_VALUE_MAX] = {0};
    property_get("vidc.debug.level", property_value, "1");
    debug_level = atoi(property_value);
    property_value[0] = '\0';
    property_get("vidc.debug.bframes", property_value, "0");
    bframes = atoi(property_value);
    property_value[0] = '\0';
    property_get("vidc.debug.entropy", property_value, "1");
    entropy = !!atoi(property_value);
    property_value[0] = '\0';
    property_get("vidc.debug.perf.mode", property_value, "0");
    perfmode = atoi(property_value);
    property_value[0] = '\0';
    property_get("vidc.debug.hybrid.hierp", property_value, "0");
    hybrid_hp = atoi(property_value);
    property_value[0] = '\0';
    handle = NULL;
}
