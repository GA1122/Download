MediaPlayerService::Client::ServiceDeathNotifier::ServiceDeathNotifier(
 const sp<IBinder>& service,
 const sp<MediaPlayerBase>& listener,
 int which) {
    mService = service;
    mListener = listener;
    mWhich = which;
}
