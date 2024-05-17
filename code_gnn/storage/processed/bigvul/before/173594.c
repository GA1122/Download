void NuPlayer::GenericSource::onSecureDecodersInstantiated(status_t err) {
 if (err != OK) {
        ALOGE("Failed to instantiate secure decoders!");
        notifyPreparedAndCleanup(err);
 return;
 }
    finishPrepareAsync();
}
