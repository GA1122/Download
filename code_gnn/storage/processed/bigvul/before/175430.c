static int adev_set_voice_volume(struct audio_hw_device *dev, float volume)
{
 int ret = 0;
 struct audio_device *adev = (struct audio_device *)dev;
    pthread_mutex_lock(&adev->lock);
  
    adev->voice_volume = volume;
    ret = set_voice_volume_l(adev, adev->voice_volume);
    pthread_mutex_unlock(&adev->lock);
 return ret;
}
