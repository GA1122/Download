static void fd_revalidate(FDrive *drv)
{
    int nb_heads, max_track, last_sect, ro;
    FDriveType drive;
    FDriveRate rate;

    FLOPPY_DPRINTF("revalidate\n");
    if (drv->blk != NULL) {
        ro = blk_is_read_only(drv->blk);
        pick_geometry(drv->blk, &nb_heads, &max_track,
                      &last_sect, drv->drive, &drive, &rate);
        if (!blk_is_inserted(drv->blk)) {
            FLOPPY_DPRINTF("No disk in drive\n");
        } else {
            FLOPPY_DPRINTF("Floppy disk (%d h %d t %d s) %s\n", nb_heads,
                           max_track, last_sect, ro ? "ro" : "rw");
        }
        if (nb_heads == 1) {
            drv->flags &= ~FDISK_DBL_SIDES;
        } else {
            drv->flags |= FDISK_DBL_SIDES;
        }
        drv->max_track = max_track;
        drv->last_sect = last_sect;
        drv->ro = ro;
        drv->drive = drive;
        drv->media_rate = rate;
    } else {
        FLOPPY_DPRINTF("No drive connected\n");
        drv->last_sect = 0;
        drv->max_track = 0;
        drv->flags &= ~FDISK_DBL_SIDES;
    }
}
