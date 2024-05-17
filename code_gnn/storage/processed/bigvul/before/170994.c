void AudioFlinger::EffectChain::dump(int fd, const Vector<String16>& args)
{
 const size_t SIZE = 256;
 char buffer[SIZE];
 String8 result;

 size_t numEffects = mEffects.size();
    snprintf(buffer, SIZE, "    %zu effects for session %d\n", numEffects, mSessionId);
    result.append(buffer);

 if (numEffects) {
 bool locked = AudioFlinger::dumpTryLock(mLock);
 if (!locked) {
            result.append("\tCould not lock mutex:\n");
 }

        result.append("\tIn buffer   Out buffer   Active tracks:\n");
        snprintf(buffer, SIZE, "\t%p  %p   %d\n",
                mInBuffer,
                mOutBuffer,
                mActiveTrackCnt);
        result.append(buffer);
        write(fd, result.string(), result.size());

 for (size_t i = 0; i < numEffects; ++i) {
            sp<EffectModule> effect = mEffects[i];
 if (effect != 0) {
                effect->dump(fd, args);
 }
 }

 if (locked) {
            mLock.unlock();
 }
 }
}
