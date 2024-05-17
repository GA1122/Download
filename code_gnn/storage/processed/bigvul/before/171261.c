void SoundTriggerHwService::Module::setCaptureState_l(bool active)
{
    ALOGV("Module::setCaptureState_l %d", active);
    sp<SoundTriggerHwService> service;
 sound_trigger_service_state_t state;

 Vector< sp<IMemory> > events;
 {
 AutoMutex lock(mLock);
        state = (active && !mDescriptor.properties.concurrent_capture) ?
                                        SOUND_TRIGGER_STATE_DISABLED : SOUND_TRIGGER_STATE_ENABLED;

 if (state == mServiceState) {
 return;
 }

        mServiceState = state;

        service = mService.promote();
 if (service == 0) {
 return;
 }

 if (state == SOUND_TRIGGER_STATE_ENABLED) {
 goto exit;
 }

 const bool supports_stop_all =
 (mHwDevice->common.version >= SOUND_TRIGGER_DEVICE_API_VERSION_1_1 &&
             mHwDevice->stop_all_recognitions);

 if (supports_stop_all) {
            mHwDevice->stop_all_recognitions(mHwDevice);
 }

 for (size_t i = 0; i < mModels.size(); i++) {
            sp<Model> model = mModels.valueAt(i);
 if (model->mState == Model::STATE_ACTIVE) {
 if (!supports_stop_all) {
                    mHwDevice->stop_recognition(mHwDevice, model->mHandle);
 }
 if (model->mType == SOUND_MODEL_TYPE_KEYPHRASE) {
 struct sound_trigger_phrase_recognition_event event;
                    memset(&event, 0, sizeof(struct sound_trigger_phrase_recognition_event));
                    event.num_phrases = model->mConfig.num_phrases;
 for (size_t i = 0; i < event.num_phrases; i++) {
                        event.phrase_extras[i] = model->mConfig.phrases[i];
 }
                    event.common.status = RECOGNITION_STATUS_ABORT;
                    event.common.type = model->mType;
                    event.common.model = model->mHandle;
                    event.common.data_size = 0;
                    sp<IMemory> eventMemory = service->prepareRecognitionEvent_l(&event.common);
 if (eventMemory != 0) {
                        events.add(eventMemory);
 }
 } else if (model->mType == SOUND_MODEL_TYPE_GENERIC) {
 struct sound_trigger_generic_recognition_event event;
                    memset(&event, 0, sizeof(struct sound_trigger_generic_recognition_event));
                    event.common.status = RECOGNITION_STATUS_ABORT;
                    event.common.type = model->mType;
                    event.common.model = model->mHandle;
                    event.common.data_size = 0;
                    sp<IMemory> eventMemory = service->prepareRecognitionEvent_l(&event.common);
 if (eventMemory != 0) {
                        events.add(eventMemory);
 }
 } else if (model->mType == SOUND_MODEL_TYPE_UNKNOWN) {
 struct sound_trigger_phrase_recognition_event event;
                    memset(&event, 0, sizeof(struct sound_trigger_phrase_recognition_event));
                    event.common.status = RECOGNITION_STATUS_ABORT;
                    event.common.type = model->mType;
                    event.common.model = model->mHandle;
                    event.common.data_size = 0;
                    sp<IMemory> eventMemory = service->prepareRecognitionEvent_l(&event.common);
 if (eventMemory != 0) {
                        events.add(eventMemory);
 }
 } else {
 goto exit;
 }
 }
 }
 }

 for (size_t i = 0; i < events.size(); i++) {
        service->sendCallbackEvent_l(new CallbackEvent(CallbackEvent::TYPE_RECOGNITION, events[i],
 this));
 }

exit:
    service->sendServiceStateEvent_l(state, this);
}
