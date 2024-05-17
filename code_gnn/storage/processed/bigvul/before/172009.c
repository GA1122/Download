int uinput_driver_check()
{
 uint32_t i;
 for (i=0; i < MAX_UINPUT_PATHS; i++)
 {
 if (access(uinput_dev_path[i], O_RDWR) == 0) {
 return 0;
 }
 }
    BTIF_TRACE_ERROR("%s ERROR: uinput device is not in the system", __FUNCTION__);
 return -1;
}
