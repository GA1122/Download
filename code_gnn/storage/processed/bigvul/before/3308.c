static void fdctrl_handle_configure(FDCtrl *fdctrl, int direction)
{
    fdctrl->config = fdctrl->fifo[2];
    fdctrl->precomp_trk =  fdctrl->fifo[3];
     
    fdctrl_reset_fifo(fdctrl);
}
