void SoundChannel::process(int event, void *info, unsigned long toggle)
{

 Mutex::Autolock lock(&mLock);

 AudioTrack::Buffer* b = NULL;
 if (event == AudioTrack::EVENT_MORE_DATA) {
       b = static_cast<AudioTrack::Buffer *>(info);
 }

 if (mToggle != toggle) {
        ALOGV("process wrong toggle %p channel %d", this, mChannelID);
 if (b != NULL) {
            b->size = 0;
 }
 return;
 }

    sp<Sample> sample = mSample;


 if (event == AudioTrack::EVENT_MORE_DATA) {

 if (b->size == 0) return;

 if (mState == IDLE) {
            b->size = 0;
 return;
 }

 if (sample != 0) {
 uint8_t* q = (uint8_t*) b->i8;
 size_t count = 0;

 if (mPos < (int)sample->size()) {
 uint8_t* p = sample->data() + mPos;
                count = sample->size() - mPos;
 if (count > b->size) {
                    count = b->size;
 }
                memcpy(q, p, count);
 } else if (mPos < mAudioBufferSize) {
                count = mAudioBufferSize - mPos;
 if (count > b->size) {
                    count = b->size;
 }
                memset(q, 0, count);
 }

            mPos += count;
            b->size = count;
 }
 } else if (event == AudioTrack::EVENT_UNDERRUN || event == AudioTrack::EVENT_BUFFER_END ||
            event == AudioTrack::EVENT_NEW_IAUDIOTRACK) {
        ALOGV("process %p channel %d event %s",
 this, mChannelID, (event == AudioTrack::EVENT_UNDERRUN) ? "UNDERRUN" :
 (event == AudioTrack::EVENT_BUFFER_END) ? "BUFFER_END" : "NEW_IAUDIOTRACK");
        mSoundPool->addToStopList(this);
 } else if (event == AudioTrack::EVENT_LOOP_END) {
        ALOGV("End loop %p channel %d", this, mChannelID);
 } else {
        ALOGW("SoundChannel::process unexpected event %d", event);
 }
}
