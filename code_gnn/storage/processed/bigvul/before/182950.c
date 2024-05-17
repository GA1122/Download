 static void setup_remaining_vcs(int src_fd, unsigned src_idx, bool utf8) {
         struct console_font_op cfo = {
                 .op = KD_FONT_OP_GET,
                 .width = UINT_MAX, .height = UINT_MAX,
                 .charcount = UINT_MAX,
         };
         struct unimapinit adv = {};
         struct unimapdesc unimapd;
         _cleanup_free_ struct unipair* unipairs = NULL;
         _cleanup_free_ void *fontbuf = NULL;
         unsigned i;
         int r;
 
         unipairs = new(struct unipair, USHRT_MAX);
         if (!unipairs) {
                 log_oom();
                 return;
         }
 
          
         r = ioctl(src_fd, KDFONTOP, &cfo);
         if (r < 0)
                 log_warning_errno(errno, "KD_FONT_OP_GET failed while trying to get the font metadata: %m");
         else {
                  
                 if (cfo.width > 32 || cfo.height > 32 || cfo.charcount > 512)
                         log_warning("Invalid font metadata - width: %u (max 32), height: %u (max 32), count: %u (max 512)",
                                     cfo.width, cfo.height, cfo.charcount);
                 else {
                          
                         fontbuf = malloc_multiply((cfo.width + 7) / 8 * 32, cfo.charcount);
                         if (!fontbuf) {
                                 log_oom();
                                 return;
                         }
                          
                         cfo.data = fontbuf;
                         r = ioctl(src_fd, KDFONTOP, &cfo);
                         if (r < 0)
                                 log_warning_errno(errno, "KD_FONT_OP_GET failed while trying to read the font data: %m");
                         else {
                                 unimapd.entries  = unipairs;
                                 unimapd.entry_ct = USHRT_MAX;
                                 r = ioctl(src_fd, GIO_UNIMAP, &unimapd);
                                 if (r < 0)
                                         log_warning_errno(errno, "GIO_UNIMAP failed while trying to read unicode mappings: %m");
                                 else
                                         cfo.op = KD_FONT_OP_SET;
                         }
                 }
         }
 
         if (cfo.op != KD_FONT_OP_SET)
                 log_warning("Fonts will not be copied to remaining consoles");
 
         for (i = 1; i <= 63; i++) {
                 char ttyname[sizeof("/dev/tty63")];
                 _cleanup_close_ int fd_d = -1;
 
                 if (i == src_idx || verify_vc_allocation(i) < 0)
                         continue;
 
                  
                 xsprintf(ttyname, "/dev/tty%u", i);
                 fd_d = open_terminal(ttyname, O_RDWR|O_CLOEXEC|O_NOCTTY);
                 if (fd_d < 0) {
                         log_warning_errno(fd_d, "Unable to open tty%u, fonts will not be copied: %m", i);
                          continue;
                  }
  
                if (verify_vc_kbmode(fd_d) < 0)
//                 if (vt_verify_kbmode(fd_d) < 0)
                          continue;
  
                  toggle_utf8(ttyname, fd_d, utf8);
 
                 if (cfo.op != KD_FONT_OP_SET)
                         continue;
 
                 r = ioctl(fd_d, KDFONTOP, &cfo);
                 if (r < 0) {
                         int last_errno, mode;
 
                          
 
                         last_errno = errno;
                         if (ioctl(fd_d, KDGETMODE, &mode) >= 0 && mode != KD_TEXT)
                                 log_debug("KD_FONT_OP_SET skipped: tty%u is not in text mode", i);
                         else
                                 log_warning_errno(last_errno, "KD_FONT_OP_SET failed, fonts will not be copied to tty%u: %m", i);
 
                         continue;
                 }
 
                  
                 r = ioctl(fd_d, PIO_UNIMAPCLR, &adv);
                 if (r < 0) {
                         log_warning_errno(errno, "PIO_UNIMAPCLR failed, unimaps might be incorrect for tty%u: %m", i);
                         continue;
                 }
 
                 r = ioctl(fd_d, PIO_UNIMAP, &unimapd);
                 if (r < 0) {
                         log_warning_errno(errno, "PIO_UNIMAP failed, unimaps might be incorrect for tty%u: %m", i);
                         continue;
                 }
 
                 log_debug("Font and unimap successfully copied to %s", ttyname);
         }
 }