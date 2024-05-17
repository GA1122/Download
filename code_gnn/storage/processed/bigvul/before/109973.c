void AudioOutputDevice::Start() {
  DCHECK(callback_) << "Initialize hasn't been called";
  message_loop()->PostTask(FROM_HERE,
      base::Bind(&AudioOutputDevice::CreateStreamOnIOThread, this,
                 audio_parameters_, input_channels_));
}
