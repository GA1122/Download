static void fdctrl_start_transfer_del(FDCtrl *fdctrl, int direction)
{
    qemu_log_mask(LOG_UNIMP, "fdctrl_start_transfer_del() unimplemented\n");

     
    fdctrl_stop_transfer(fdctrl, FD_SR0_ABNTERM | FD_SR0_SEEK, 0x00, 0x00);
}
