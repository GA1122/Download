static void AudioRecordCallbackFunction(int event, void *user, void *info) {
 AudioSource *source = (AudioSource *) user;
 switch (event) {
 case AudioRecord::EVENT_MORE_DATA: {
            source->dataCallback(*((AudioRecord::Buffer *) info));
 break;
 }
 case AudioRecord::EVENT_OVERRUN: {
            ALOGW("AudioRecord reported overrun!");
 break;
 }
 default:
 break;
 }
}
