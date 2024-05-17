void ExpectTimeoutAndClose(scoped_refptr<UsbDeviceHandle> handle,
                           const base::Closure& quit_closure,
                           UsbTransferStatus status,
                           scoped_refptr<net::IOBuffer> buffer,
                           size_t transferred) {
  EXPECT_EQ(UsbTransferStatus::TIMEOUT, status);
  handle->Close();
  quit_closure.Run();
}
