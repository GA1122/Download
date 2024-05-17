static void fdctrl_handle_seek(FDCtrl *fdctrl, int direction)
{
    FDrive *cur_drv;

    SET_CUR_DRV(fdctrl, fdctrl->fifo[1] & FD_DOR_SELMASK);
    cur_drv = get_cur_drv(fdctrl);
    fdctrl_reset_fifo(fdctrl);
     
    fd_seek(cur_drv, cur_drv->head, fdctrl->fifo[2], cur_drv->sect, 1);
     
    fdctrl->status0 |= FD_SR0_SEEK;
    fdctrl_raise_irq(fdctrl);
}
