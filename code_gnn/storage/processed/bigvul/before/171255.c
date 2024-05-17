void SoundTriggerHwService::CallbackThread::sendCallbackEvent(
 const sp<SoundTriggerHwService::CallbackEvent>& event)
{
 AutoMutex lock(mCallbackLock);
    mEventQueue.add(event);
    mCallbackCond.signal();
}
