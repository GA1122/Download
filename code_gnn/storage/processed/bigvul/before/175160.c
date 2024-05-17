void NuPlayer::GenericSource::notifyBufferingUpdate(int percentage) {
    sp<AMessage> msg = dupNotify();
    msg->setInt32("what", kWhatBufferingUpdate);
    msg->setInt32("percentage", percentage);
    msg->post();
}
