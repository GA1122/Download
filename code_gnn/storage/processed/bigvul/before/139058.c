void AudioOutputAuthorizationHandler::DeviceParametersReceived(
    AuthorizationCompletedCallback cb,
    bool should_send_id,
    const std::string& raw_device_id,
    const media::AudioParameters& output_params) const {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DCHECK(!raw_device_id.empty());

  cb.Run(media::OUTPUT_DEVICE_STATUS_OK, should_send_id,
         output_params.IsValid() ? output_params
                                 : TryToFixAudioParameters(output_params),
         raw_device_id);
}
