void AudioSystemImpl::GetInputStreamParameters(
    const std::string& device_id,
    OnAudioParamsCallback on_params_cb) const {
  if (GetTaskRunner()->BelongsToCurrentThread()) {
    GetTaskRunner()->PostTask(
        FROM_HERE, base::Bind(on_params_cb, GetInputParametersOnDeviceThread(
                                                audio_manager_, device_id)));
    return;
  }
  base::PostTaskAndReplyWithResult(
      GetTaskRunner(), FROM_HERE,
      base::Bind(&GetInputParametersOnDeviceThread,
                 base::Unretained(audio_manager_), device_id),
       std::move(on_params_cb));
 }
