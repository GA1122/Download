PepperPlatformAudioInput* PepperPlatformAudioInput::Create(
    int render_frame_id,
    const std::string& device_id,
    const GURL& document_url,
    int sample_rate,
    int frames_per_buffer,
    PepperAudioInputHost* client) {
  scoped_refptr<PepperPlatformAudioInput> audio_input(
      new PepperPlatformAudioInput());
  if (audio_input->Initialize(render_frame_id,
                              device_id,
                              document_url,
                              sample_rate,
                              frames_per_buffer,
                              client)) {
    audio_input->AddRef();
    return audio_input.get();
  }
  return NULL;
}
