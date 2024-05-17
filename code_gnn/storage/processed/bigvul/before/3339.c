static int fdctrl_seek_to_next_sect(FDCtrl *fdctrl, FDrive *cur_drv)
{
    FLOPPY_DPRINTF("seek to next sector (%d %02x %02x => %d)\n",
                   cur_drv->head, cur_drv->track, cur_drv->sect,
                   fd_sector(cur_drv));
     
    uint8_t new_head = cur_drv->head;
    uint8_t new_track = cur_drv->track;
    uint8_t new_sect = cur_drv->sect;

    int ret = 1;

    if (new_sect >= cur_drv->last_sect ||
        new_sect == fdctrl->eot) {
        new_sect = 1;
        if (FD_MULTI_TRACK(fdctrl->data_state)) {
            if (new_head == 0 &&
                (cur_drv->flags & FDISK_DBL_SIDES) != 0) {
                new_head = 1;
            } else {
                new_head = 0;
                new_track++;
                fdctrl->status0 |= FD_SR0_SEEK;
                if ((cur_drv->flags & FDISK_DBL_SIDES) == 0) {
                    ret = 0;
                }
            }
        } else {
            fdctrl->status0 |= FD_SR0_SEEK;
            new_track++;
            ret = 0;
        }
        if (ret == 1) {
            FLOPPY_DPRINTF("seek to next track (%d %02x %02x => %d)\n",
                    new_head, new_track, new_sect, fd_sector(cur_drv));
        }
    } else {
        new_sect++;
    }
    fd_seek(cur_drv, new_head, new_track, new_sect, 1);
    return ret;
}
