void MediaPlayerService::AudioOutput::deleteRecycledTrack_l()
{
    ALOGV("deleteRecycledTrack_l");
 if (mRecycledTrack != 0) {

 if (mCallbackData != NULL) {
            mCallbackData->setOutput(NULL);
            mCallbackData->endTrackSwitch();
 }

 if ((mRecycledTrack->getFlags() & AUDIO_OUTPUT_FLAG_COMPRESS_OFFLOAD) == 0) {
 int32_t msec = 0;
 if (!mRecycledTrack->stopped()) {  
 (void)mRecycledTrack->pendingDuration(&msec);
 }
            mRecycledTrack->stop();  
            ALOGD("deleting recycled track, waiting for data drain (%d msec)", msec);
 if (msec > 0) {
 static const int32_t WAIT_LIMIT_MS = 3000;
 if (msec > WAIT_LIMIT_MS) {
                    msec = WAIT_LIMIT_MS;
 }
                usleep(msec * 1000LL);
 }
 }

        mRecycledTrack.clear();
        close_l();
 delete mCallbackData;
        mCallbackData = NULL;
 }
}
