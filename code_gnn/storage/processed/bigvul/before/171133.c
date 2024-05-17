void MediaPlayerService::addBatteryData(uint32_t params)
{
 Mutex::Autolock lock(mLock);

 int32_t time = systemTime() / 1000000L;

 if ((params & kBatteryDataSpeakerOn)
 || (params & kBatteryDataOtherAudioDeviceOn)) {

 int deviceOn[NUM_AUDIO_DEVICES];
 for (int i = 0; i < NUM_AUDIO_DEVICES; i++) {
            deviceOn[i] = 0;
 }

 if ((params & kBatteryDataSpeakerOn)
 && (params & kBatteryDataOtherAudioDeviceOn)) {
            deviceOn[SPEAKER_AND_OTHER] = 1;
 } else if (params & kBatteryDataSpeakerOn) {
            deviceOn[SPEAKER] = 1;
 } else {
            deviceOn[OTHER_AUDIO_DEVICE] = 1;
 }

 for (int i = 0; i < NUM_AUDIO_DEVICES; i++) {
 if (mBatteryAudio.deviceOn[i] != deviceOn[i]){

 if (mBatteryAudio.refCount > 0) {  
 if (!deviceOn[i]) {
                        mBatteryAudio.lastTime[i] += time;
                        mBatteryAudio.totalTime[i] += mBatteryAudio.lastTime[i];
                        mBatteryAudio.lastTime[i] = 0;
 } else {
                        mBatteryAudio.lastTime[i] = 0 - time;
 }
 }

                mBatteryAudio.deviceOn[i] = deviceOn[i];
 }
 }
 return;
 }

 if (params & kBatteryDataAudioFlingerStart) {
 if (mBatteryAudio.refCount == 0) {
 for (int i = 0; i < NUM_AUDIO_DEVICES; i++) {
 if (mBatteryAudio.deviceOn[i]) {
                    mBatteryAudio.lastTime[i] -= time;
 }
 }
 }

        mBatteryAudio.refCount ++;
 return;

 } else if (params & kBatteryDataAudioFlingerStop) {
 if (mBatteryAudio.refCount <= 0) {
            ALOGW("Battery track warning: refCount is <= 0");
 return;
 }

 if (mBatteryAudio.refCount == 1) {
 for (int i = 0; i < NUM_AUDIO_DEVICES; i++) {
 if (mBatteryAudio.deviceOn[i]) {
                    mBatteryAudio.lastTime[i] += time;
                    mBatteryAudio.totalTime[i] += mBatteryAudio.lastTime[i];
                    mBatteryAudio.lastTime[i] = 0;
 }
 }
 }

        mBatteryAudio.refCount --;
 return;
 }

 int uid = IPCThreadState::self()->getCallingUid();
 if (uid == AID_MEDIA) {
 return;
 }
 int index = mBatteryData.indexOfKey(uid);

 if (index < 0) {  
 BatteryUsageInfo info;
        info.audioTotalTime = 0;
        info.videoTotalTime = 0;
        info.audioLastTime = 0;
        info.videoLastTime = 0;
        info.refCount = 0;

 if (mBatteryData.add(uid, info) == NO_MEMORY) {
            ALOGE("Battery track error: no memory for new app");
 return;
 }
 }

 BatteryUsageInfo &info = mBatteryData.editValueFor(uid);

 if (params & kBatteryDataCodecStarted) {
 if (params & kBatteryDataTrackAudio) {
            info.audioLastTime -= time;
            info.refCount ++;
 }
 if (params & kBatteryDataTrackVideo) {
            info.videoLastTime -= time;
            info.refCount ++;
 }
 } else {
 if (info.refCount == 0) {
            ALOGW("Battery track warning: refCount is already 0");
 return;
 } else if (info.refCount < 0) {
            ALOGE("Battery track error: refCount < 0");
            mBatteryData.removeItem(uid);
 return;
 }

 if (params & kBatteryDataTrackAudio) {
            info.audioLastTime += time;
            info.refCount --;
 }
 if (params & kBatteryDataTrackVideo) {
            info.videoLastTime += time;
            info.refCount --;
 }

 if (info.refCount == 0) {
            info.audioTotalTime += info.audioLastTime;
            info.audioLastTime = 0;
            info.videoTotalTime += info.videoLastTime;
            info.videoLastTime = 0;
 }
 }
}
