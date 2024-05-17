void VideoEncodeAcceleratorClient::NotifyError(
    VideoEncodeAccelerator::Error error) {
  DVLOG(2) << __func__;
  client_->NotifyError(error);
}
