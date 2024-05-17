void MediaPlayerService::Client::ServiceDeathNotifier::binderDied(const wp<IBinder>&  ) {
    sp<MediaPlayerBase> listener = mListener.promote();
 if (listener != NULL) {
        listener->sendEvent(MEDIA_ERROR, MEDIA_ERROR_SERVER_DIED, mWhich);
 } else {
        ALOGW("listener for process %d death is gone", mWhich);
 }
}
