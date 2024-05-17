static int close_bluetooth_stack(struct hw_device_t* device)
{
    UNUSED(device);
    cleanup();
 return 0;
}
