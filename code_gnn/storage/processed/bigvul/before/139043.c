  void SetResult(UsbTransferStatus status,
                 scoped_refptr<net::IOBuffer> buffer,
                 size_t transferred) {
    status_ = status;
    transferred_ = transferred;
     run_loop_.Quit();
   }
