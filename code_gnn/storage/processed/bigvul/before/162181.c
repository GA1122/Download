void MojoAudioOutputIPC::ReceivedDeviceAuthorization(
    media::OutputDeviceStatus status,
    const media::AudioParameters& params,
    const std::string& device_id) const {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  DCHECK(delegate_);
  delegate_->OnDeviceAuthorized(status, params, device_id);
}
