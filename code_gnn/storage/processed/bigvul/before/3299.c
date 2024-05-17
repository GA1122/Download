static int fd_sector(FDrive *drv)
{
    return fd_sector_calc(drv->head, drv->track, drv->sect, drv->last_sect,
                          NUM_SIDES(drv));
}
