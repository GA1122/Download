static void fdctrl_handle_version(FDCtrl *fdctrl, int direction)
{
     
    fdctrl->fifo[0] = fdctrl->version;
    fdctrl_set_fifo(fdctrl, 1);
}
