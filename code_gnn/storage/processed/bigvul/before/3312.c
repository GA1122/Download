static void fdctrl_handle_partid(FDCtrl *fdctrl, int direction)
{
    fdctrl->fifo[0] = 0x41;  
    fdctrl_set_fifo(fdctrl, 1);
}
