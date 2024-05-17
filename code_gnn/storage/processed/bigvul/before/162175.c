bool MojoAudioOutputIPC::DoRequestDeviceAuthorization(
    int session_id,
    const std::string& device_id,
    AuthorizationCB callback) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  auto* factory = factory_accessor_.Run();
  if (!factory) {
    LOG(ERROR) << "MojoAudioOutputIPC failed to acquire factory";

    callback.Reset();
    return false;
  }

  static_assert(sizeof(int) == sizeof(int32_t),
                "sizeof(int) == sizeof(int32_t)");
  factory->RequestDeviceAuthorization(MakeProviderRequest(), session_id,
                                      device_id, std::move(callback));
  return true;
}
