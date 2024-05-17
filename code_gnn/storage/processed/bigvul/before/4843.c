GetXI2MaskByte(XI2Mask *mask, DeviceIntPtr dev, int event_type)
{
     
    if (xi2mask_isset(mask, dev, event_type))
        return event_get_filter_from_xi2type(event_type);
    else
        return 0;
}
