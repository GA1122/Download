void SoundTriggerHwService::sendCallbackEvent_l(const sp<CallbackEvent>& event)
{
    mCallbackThread->sendCallbackEvent(event);
}
