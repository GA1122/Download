static void fdctrl_reset_fifo(FDCtrl *fdctrl)
{
    fdctrl->data_dir = FD_DIR_WRITE;
    fdctrl->data_pos = 0;
    fdctrl->msr &= ~(FD_MSR_CMDBUSY | FD_MSR_DIO);
}
