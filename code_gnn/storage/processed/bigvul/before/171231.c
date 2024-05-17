MediaPlayerService::Client::ServiceDeathNotifier::~ServiceDeathNotifier() {
    mService->unlinkToDeath(this);
}
