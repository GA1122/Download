void NuPlayer::NuPlayerStreamListener::issueCommand(
 Command cmd, bool synchronous, const sp<AMessage> &extra) {
    CHECK(!synchronous);

 QueueEntry entry;
    entry.mIsCommand = true;
    entry.mCommand = cmd;
    entry.mExtra = extra;

 Mutex::Autolock autoLock(mLock);
    mQueue.push_back(entry);

 if (mSendDataNotification) {
        mSendDataNotification = false;

 if (mTargetHandler != NULL) {
 (new AMessage(kWhatMoreDataQueued, mTargetHandler))->post();
 }
 }
}
