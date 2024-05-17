void AudioRendererHost::OnCreateStream(int stream_id,
                                       int render_frame_id,
                                       const media::AudioParameters& params) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DVLOG(1) << "AudioRendererHost@" << this << "::OnCreateStream"
           << "(stream_id=" << stream_id << ")";

  std::string device_unique_id;
  const auto& auth_data = authorizations_.find(stream_id);
  if (auth_data != authorizations_.end()) {
    if (!auth_data->second.first) {
      content::bad_message::ReceivedBadMessage(
          this, bad_message::ARH_CREATED_STREAM_WITHOUT_AUTHORIZATION);
      return;
    }
    device_unique_id.swap(auth_data->second.second);
    authorizations_.erase(auth_data);
  }

  if (LookupById(stream_id)) {
    SendErrorMessage(stream_id);
    return;
  }
  if (render_frame_id <= 0) {
    SendErrorMessage(stream_id);
    return;
  }

  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      base::Bind(validate_render_frame_id_function_, render_process_id_,
                 render_frame_id,
                 base::Bind(&AudioRendererHost::DidValidateRenderFrame, this,
                            stream_id)));

  MediaObserver* const media_observer =
      GetContentClient()->browser()->GetMediaObserver();

  MediaInternals* const media_internals = MediaInternals::GetInstance();
  std::unique_ptr<media::AudioLog> audio_log = media_internals->CreateAudioLog(
      media::AudioLogFactory::AUDIO_OUTPUT_CONTROLLER);
  media_internals->SetWebContentsTitleForAudioLogEntry(
      stream_id, render_process_id_, render_frame_id, audio_log.get());
  delegates_.push_back(
      base::WrapUnique<AudioOutputDelegate>(new AudioOutputDelegateImpl(
          this, audio_manager_, std::move(audio_log), mirroring_manager_,
          media_observer, stream_id, render_frame_id, render_process_id_,
          params, device_unique_id)));
}
