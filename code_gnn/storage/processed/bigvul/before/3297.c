static void fd_init(FDrive *drv)
{
     
    drv->drive = FDRIVE_DRV_NONE;
    drv->perpendicular = 0;
     
    drv->last_sect = 0;
    drv->max_track = 0;
}
