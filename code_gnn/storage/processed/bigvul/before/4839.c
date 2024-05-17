GetEventMask(DeviceIntPtr dev, xEvent *event, InputClients * other)
{
    int evtype;

     
    if ((evtype = xi2_get_type(event))) {
        return GetXI2MaskByte(other->xi2mask, dev, evtype);
    }
    else if (core_get_type(event) != 0)
        return other->mask[XIAllDevices];
    else
        return other->mask[dev->id];
}
