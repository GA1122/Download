NuPlayer::GenericSource::~GenericSource() {
 if (mLooper != NULL) {
        mLooper->unregisterHandler(id());
        mLooper->stop();
 }
}
