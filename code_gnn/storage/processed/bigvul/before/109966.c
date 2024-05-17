void AudioOutputDevice::Pause(bool flush) {
  message_loop()->PostTask(FROM_HERE,
      base::Bind(&AudioOutputDevice::PauseOnIOThread, this, flush));
}
