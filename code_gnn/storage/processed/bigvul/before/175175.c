void NuPlayer::GenericSource::schedulePollBuffering() {
    sp<AMessage> msg = new AMessage(kWhatPollBuffering, id());
    msg->setInt32("generation", mPollBufferingGeneration);
    msg->post(1000000ll);
}
