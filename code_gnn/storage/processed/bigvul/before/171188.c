status_t MediaPlayerService::pullBatteryData(Parcel* reply) {
 Mutex::Autolock lock(mLock);

 int32_t time = systemTime() / 1000000L;  
 int32_t totalTime;

 for (int i = 0; i < NUM_AUDIO_DEVICES; i++) {
        totalTime = mBatteryAudio.totalTime[i];

 if (mBatteryAudio.deviceOn[i]
 && (mBatteryAudio.lastTime[i] != 0)) {
 int32_t tmpTime = mBatteryAudio.lastTime[i] + time;
                totalTime += tmpTime;
 }

        reply->writeInt32(totalTime);
        mBatteryAudio.totalTime[i] = 0;
 }

 BatteryUsageInfo info;
 int size = mBatteryData.size();

    reply->writeInt32(size);
 int i = 0;

 while (i < size) {
        info = mBatteryData.valueAt(i);

        reply->writeInt32(mBatteryData.keyAt(i));  
        reply->writeInt32(info.audioTotalTime);
        reply->writeInt32(info.videoTotalTime);

        info.audioTotalTime = 0;
        info.videoTotalTime = 0;

 if (info.refCount <= 0) {
            mBatteryData.removeItemsAt(i);
            size --;
            i --;
 }
        i++;
 }
 return NO_ERROR;
}
