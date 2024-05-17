void CameraService::playSound(sound_kind kind) {
    LOG1("playSound(%d)", kind);
 Mutex::Autolock lock(mSoundLock);
    sp<MediaPlayer> player = mSoundPlayer[kind];
 if (player != 0) {
        player->seekTo(0);
        player->start();
 }
}
