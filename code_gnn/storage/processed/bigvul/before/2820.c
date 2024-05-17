gx_device_set_procs(gx_device * dev)
{
    if (dev->static_procs != 0) {	 
        dev->procs = *dev->static_procs;
        dev->static_procs = 0;
    }
}
