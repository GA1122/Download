void AudioOutputAuthorizationHandler::RequestDeviceAuthorization(
    int render_frame_id,
    int session_id,
    const std::string& device_id,
    const url::Origin& security_origin,
    AuthorizationCompletedCallback cb) const {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  if (!IsValidDeviceId(device_id)) {
    cb.Run(media::OUTPUT_DEVICE_STATUS_ERROR_NOT_FOUND, false,
           media::AudioParameters::UnavailableDeviceParams(), std::string());
    return;
  }

  if (media::AudioDeviceDescription::UseSessionIdToSelectDevice(session_id,
                                                                device_id)) {
    const StreamDeviceInfo* info =
        media_stream_manager_->audio_input_device_manager()
            ->GetOpenedDeviceInfoById(session_id);
    if (info && !info->device.matched_output_device_id.empty()) {
      media::AudioParameters output_params(
          media::AudioParameters::AUDIO_PCM_LOW_LATENCY,
          static_cast<media::ChannelLayout>(
              info->device.matched_output.channel_layout),
          info->device.matched_output.sample_rate, 16,
          info->device.matched_output.frames_per_buffer);
      output_params.set_effects(info->device.matched_output.effects);

      DeviceParametersReceived(std::move(cb), true,
                               info->device.matched_output_device_id,
                               output_params);
      return;
    }
  }

  if (media::AudioDeviceDescription::IsDefaultDevice(device_id)) {
    AccessChecked(std::move(cb), device_id, security_origin, true);
    return;
  }

  if (!MediaStreamManager::IsOriginAllowed(render_process_id_,
                                           security_origin)) {
    bad_message::ReceivedBadMessage(render_process_id_,
                                    bad_message::AOAH_UNAUTHORIZED_URL);
    return;
  }

  permission_checker_->CheckPermission(
      MEDIA_DEVICE_TYPE_AUDIO_OUTPUT, render_process_id_, render_frame_id,
      security_origin,
      base::Bind(&AudioOutputAuthorizationHandler::AccessChecked,
                 weak_factory_.GetWeakPtr(), std::move(cb), device_id,
                 security_origin));
}
