void MediaPlayerService::AudioOutput::CallbackWrapper(
 int event, void *cookie, void *info) {
 CallbackData *data = (CallbackData*)cookie;
    data->lock();
 AudioOutput *me = data->getOutput();
 AudioTrack::Buffer *buffer = (AudioTrack::Buffer *)info;
 if (me == NULL) {
        data->unlock();
 if (buffer != NULL) {
            buffer->size = 0;
 }
 return;
 }

 switch(event) {
 case AudioTrack::EVENT_MORE_DATA: {
 size_t actualSize = (*me->mCallback)(
                me, buffer->raw, buffer->size, me->mCallbackCookie,
                CB_EVENT_FILL_BUFFER);


        ALOGV_IF(actualSize == 0 && buffer->size > 0, "callbackwrapper: empty buffer returned");

        buffer->size = actualSize;
 } break;

 case AudioTrack::EVENT_STREAM_END:
        ALOGV("callbackwrapper: deliver EVENT_STREAM_END");
 (*me->mCallback)(me, NULL  , 0  ,
                me->mCallbackCookie, CB_EVENT_STREAM_END);
 break;

 case AudioTrack::EVENT_NEW_IAUDIOTRACK :
        ALOGV("callbackwrapper: deliver EVENT_TEAR_DOWN");
 (*me->mCallback)(me,  NULL  , 0  ,
                me->mCallbackCookie, CB_EVENT_TEAR_DOWN);
 break;

 case AudioTrack::EVENT_UNDERRUN:
        ALOGD("callbackwrapper: EVENT_UNDERRUN (discarded)");
 break;

 default:
        ALOGE("received unknown event type: %d inside CallbackWrapper !", event);
 }

    data->unlock();
}
