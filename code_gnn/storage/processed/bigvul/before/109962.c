void AudioOutputDevice::Initialize(const AudioParameters& params,
                                   RenderCallback* callback) {
  CHECK_EQ(0, stream_id_) <<
      "AudioOutputDevice::Initialize() must be called before Start()";

  CHECK(!callback_);   

  audio_parameters_ = params;
  callback_ = callback;
}
