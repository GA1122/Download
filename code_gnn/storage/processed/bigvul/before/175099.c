void SoundChannel::stop_l()
{
 if (doStop_l()) {
        mSoundPool->done_l(this);
 }
}
