bool AudioOutputDevice::SetVolume(double volume) {
  if (volume < 0 || volume > 1.0)
    return false;

  if (!message_loop()->PostTask(FROM_HERE,
          base::Bind(&AudioOutputDevice::SetVolumeOnIOThread, this, volume))) {
    return false;
  }

  return true;
}
