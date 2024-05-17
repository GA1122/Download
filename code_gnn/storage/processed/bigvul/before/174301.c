void NuPlayer::NuPlayerStreamListener::queueBuffer(size_t index, size_t size) {
 QueueEntry entry;
    entry.mIsCommand = false;
    entry.mIndex = index;
    entry.mSize = size;
    entry.mOffset = 0;

 Mutex::Autolock autoLock(mLock);
    mQueue.push_back(entry);

 if (mSendDataNotification) {
        mSendDataNotification = false;

 if (mTargetHandler != NULL) {
 (new AMessage(kWhatMoreDataQueued, mTargetHandler))->post();
 }
 }
}
