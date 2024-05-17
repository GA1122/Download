bool PepperPlatformAudioInput::Initialize(
    int render_frame_id,
    const std::string& device_id,
    const GURL& document_url,
    int sample_rate,
    int frames_per_buffer,
    PepperAudioInputHost* client) {
  DCHECK(main_message_loop_proxy_->BelongsToCurrentThread());

  RenderFrameImpl* const render_frame =
      RenderFrameImpl::FromRoutingID(render_frame_id);
  if (!render_frame || !client)
    return false;

  render_frame_id_ = render_frame_id;
  client_ = client;

  if (!GetMediaDeviceManager())
    return false;

  ipc_ = RenderThreadImpl::current()
             ->audio_input_message_filter()
             ->CreateAudioInputIPC(render_frame->render_view()->GetRoutingID());

  params_.Reset(media::AudioParameters::AUDIO_PCM_LINEAR,
                media::CHANNEL_LAYOUT_MONO,
                ppapi::kAudioInputChannels,
                sample_rate,
                ppapi::kBitsPerAudioInputSample,
                frames_per_buffer);

  pending_open_device_id_ = GetMediaDeviceManager()->OpenDevice(
      PP_DEVICETYPE_DEV_AUDIOCAPTURE,
      device_id.empty() ? media::AudioManagerBase::kDefaultDeviceId : device_id,
      document_url,
      base::Bind(&PepperPlatformAudioInput::OnDeviceOpened, this));
  pending_open_device_ = true;

  return true;
}
