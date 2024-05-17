void DXVAVideoDecodeAccelerator::NotifyInitializeDone() {
  if (client_)
    client_->NotifyInitializeDone();
}
