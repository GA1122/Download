void AudioOutputDevice::Play() {
  message_loop()->PostTask(FROM_HERE,
      base::Bind(&AudioOutputDevice::PlayOnIOThread, this));
}
