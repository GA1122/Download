SoundTriggerHwService::CallbackThread::~CallbackThread()
{
 while (!mEventQueue.isEmpty()) {
        mEventQueue[0]->mMemory.clear();
        mEventQueue.removeAt(0);
 }
}
