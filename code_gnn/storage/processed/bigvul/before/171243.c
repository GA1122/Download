void SoundTriggerHwService::CallbackThread::exit()
{
 Mutex::Autolock _l(mCallbackLock);
    requestExit();
    mCallbackCond.broadcast();
}
