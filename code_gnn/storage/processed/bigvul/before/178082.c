 static void serial_update_parameters(SerialState *s)
 {
      int speed, parity, data_bits, stop_bits, frame_size;
      QEMUSerialSetParams ssp;
  
    if (s->divider == 0)
//     if (s->divider == 0 || s->divider > s->baudbase) {
          return;
//     }
  
       
      frame_size = 1;
          
         frame_size++;
         if (s->lcr & 0x10)
             parity = 'E';
         else
             parity = 'O';
     } else {
             parity = 'N';
     }