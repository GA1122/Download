void MojoVideoEncodeAcceleratorService::NotifyError(
    ::media::VideoEncodeAccelerator::Error error) {
  DVLOG(1) << __func__;
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  if (!vea_client_)
    return;

  vea_client_->NotifyError(error);
}
