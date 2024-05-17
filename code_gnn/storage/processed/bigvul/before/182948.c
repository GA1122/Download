  int vt_reset_keyboard(int fd) {
        int kb;
//         int kb, r;
  
           
          kb = vt_default_utf8() != 0 ? K_UNICODE : K_XLATE;
  
//         r = vt_verify_kbmode(fd);
//         if (r == -EBUSY) {
//                 log_debug_errno(r, "Keyboard is not in XLATE or UNICODE mode, not resetting: %m");
//                 return 0;
//         } else if (r < 0)
//                 return r;
// 
          if (ioctl(fd, KDSKBMODE, kb) < 0)
                  return -errno;
  
         return 0;
 }