void PPB_URLLoader_Impl::didDownloadData(WebURLLoader* loader,
                                         int data_length) {
  bytes_received_ += data_length;
  UpdateStatus();
}
