 ssize_t nbd_wr_syncv(QIOChannel *ioc,
                      struct iovec *iov,
                      size_t niov,
                      size_t length,
                      bool do_read)
 {
     ssize_t done = 0;
     Error *local_err = NULL;
     struct iovec *local_iov = g_new(struct iovec, niov);
     struct iovec *local_iov_head = local_iov;
     unsigned int nlocal_iov = niov;
 
     nlocal_iov = iov_copy(local_iov, nlocal_iov, iov, niov, 0, length);
 
     while (nlocal_iov > 0) {
         ssize_t len;
         if (do_read) {
             len = qio_channel_readv(ioc, local_iov, nlocal_iov, &local_err);
         } else {
             len = qio_channel_writev(ioc, local_iov, nlocal_iov, &local_err);
          }
          if (len == QIO_CHANNEL_ERR_BLOCK) {
              if (qemu_in_coroutine()) {
                 
                qemu_coroutine_yield();
            } else if (done) {
                 
                qio_channel_wait(ioc,
                                 do_read ? G_IO_IN : G_IO_OUT);
//                 qio_channel_yield(ioc, do_read ? G_IO_IN : G_IO_OUT);
              } else {
                  return -EAGAIN;
              }
             } else if (done) {
                  
                 qio_channel_wait(ioc,
                                  do_read ? G_IO_IN : G_IO_OUT);
             } else {
                 return -EAGAIN;
             }
             continue;
         }
         if (len < 0) {
             TRACE("I/O error: %s", error_get_pretty(local_err));
             error_free(local_err);
              
             done = -EIO;
             goto cleanup;
         }
 
         if (do_read && len == 0) {
             break;
         }
 
         iov_discard_front(&local_iov, &nlocal_iov, len);
         done += len;
     }