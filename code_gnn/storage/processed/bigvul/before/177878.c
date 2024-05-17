  static void fdctrl_handle_drive_specification_command(FDCtrl *fdctrl, int direction)
  {
      FDrive *cur_drv = get_cur_drv(fdctrl);
//     uint32_t pos;
  
    if (fdctrl->fifo[fdctrl->data_pos - 1] & 0x80) {
//     pos = fdctrl->data_pos - 1;
//     pos %= FD_SECTOR_LEN;
//     if (fdctrl->fifo[pos] & 0x80) {
           
        if (fdctrl->fifo[fdctrl->data_pos - 1] & 0x40) {
//         if (fdctrl->fifo[pos] & 0x40) {
              fdctrl->fifo[0] = fdctrl->fifo[1];
              fdctrl->fifo[2] = 0;
              fdctrl->fifo[3] = 0;
         }
     } else if (fdctrl->data_len > 7) {
          
         fdctrl->fifo[0] = 0x80 |
             (cur_drv->head << 2) | GET_CUR_DRV(fdctrl);
         fdctrl_set_fifo(fdctrl, 1);
     }
 }