IsKeyboardDevice(DeviceIntPtr dev)
{
    return (dev->type == MASTER_KEYBOARD) ||
        ((dev->key && dev->kbdfeed) && !IsPointerDevice(dev));
}
