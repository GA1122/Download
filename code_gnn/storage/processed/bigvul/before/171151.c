status_t MediaPlayerService::AudioOutput::dump(int fd, const Vector<String16>& args) const
{
 const size_t SIZE = 256;
 char buffer[SIZE];
 String8 result;

    result.append(" AudioOutput\n");
    snprintf(buffer, 255, "  stream type(%d), left - right volume(%f, %f)\n",
            mStreamType, mLeftVolume, mRightVolume);
    result.append(buffer);
    snprintf(buffer, 255, "  msec per frame(%f), latency (%d)\n",
            mMsecsPerFrame, (mTrack != 0) ? mTrack->latency() : -1);
    result.append(buffer);
    snprintf(buffer, 255, "  aux effect id(%d), send level (%f)\n",
            mAuxEffectId, mSendLevel);
    result.append(buffer);

 ::write(fd, result.string(), result.size());
 if (mTrack != 0) {
        mTrack->dump(fd, args);
 }
 return NO_ERROR;
}
