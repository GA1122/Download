void MojoAudioOutputIPC::RequestDeviceAuthorization(
    media::AudioOutputIPCDelegate* delegate,
    int session_id,
    const std::string& device_id,
    const url::Origin& security_origin) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  DCHECK(delegate);
  DCHECK(!delegate_);
  DCHECK(!AuthorizationRequested());
  DCHECK(!StreamCreationRequested());
  delegate_ = delegate;

  DoRequestDeviceAuthorization(
      session_id, device_id,
      mojo::WrapCallbackWithDefaultInvokeIfNotRun(
          base::BindOnce(&MojoAudioOutputIPC::ReceivedDeviceAuthorization,
                         weak_factory_.GetWeakPtr()),
          media::OutputDeviceStatus::OUTPUT_DEVICE_STATUS_ERROR_INTERNAL,
          media::AudioParameters::UnavailableDeviceParams(), std::string()));
}
