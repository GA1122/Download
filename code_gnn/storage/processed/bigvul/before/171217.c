void MediaPlayerService::AudioOutput::setVolume(float left, float right)
{
    ALOGV("setVolume(%f, %f)", left, right);
 Mutex::Autolock lock(mLock);
    mLeftVolume = left;
    mRightVolume = right;
 if (mTrack != 0) {
        mTrack->setVolume(left, right);
 }
}
