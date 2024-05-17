static int fd_seek(FDrive *drv, uint8_t head, uint8_t track, uint8_t sect,
                   int enable_seek)
{
    uint32_t sector;
    int ret;

    if (track > drv->max_track ||
        (head != 0 && (drv->flags & FDISK_DBL_SIDES) == 0)) {
        FLOPPY_DPRINTF("try to read %d %02x %02x (max=%d %d %02x %02x)\n",
                       head, track, sect, 1,
                       (drv->flags & FDISK_DBL_SIDES) == 0 ? 0 : 1,
                       drv->max_track, drv->last_sect);
        return 2;
    }
    if (sect > drv->last_sect) {
        FLOPPY_DPRINTF("try to read %d %02x %02x (max=%d %d %02x %02x)\n",
                       head, track, sect, 1,
                       (drv->flags & FDISK_DBL_SIDES) == 0 ? 0 : 1,
                       drv->max_track, drv->last_sect);
        return 3;
    }
    sector = fd_sector_calc(head, track, sect, drv->last_sect, NUM_SIDES(drv));
    ret = 0;
    if (sector != fd_sector(drv)) {
#if 0
        if (!enable_seek) {
            FLOPPY_DPRINTF("error: no implicit seek %d %02x %02x"
                           " (max=%d %02x %02x)\n",
                           head, track, sect, 1, drv->max_track,
                           drv->last_sect);
            return 4;
        }
#endif
        drv->head = head;
        if (drv->track != track) {
            if (drv->blk != NULL && blk_is_inserted(drv->blk)) {
                drv->media_changed = 0;
            }
            ret = 1;
        }
        drv->track = track;
        drv->sect = sect;
    }

    if (drv->blk == NULL || !blk_is_inserted(drv->blk)) {
        ret = 2;
    }

    return ret;
}
