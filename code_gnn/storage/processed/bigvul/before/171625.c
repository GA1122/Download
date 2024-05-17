static int adev_close(hw_device_t *device)
{
    FNLOG();

    free(device);
 return 0;
}
