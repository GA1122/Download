void NuPlayer::GenericSource::prepareAsync() {
 if (mLooper == NULL) {
        mLooper = new ALooper;
        mLooper->setName("generic");
        mLooper->start();

        mLooper->registerHandler(this);
 }

    sp<AMessage> msg = new AMessage(kWhatPrepareAsync, id());
    msg->post();
}
