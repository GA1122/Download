MediaPlayer* CameraService::newMediaPlayer(const char *file) {
 MediaPlayer* mp = new MediaPlayer();
 if (mp->setDataSource(file, NULL) == NO_ERROR) {
        mp->setAudioStreamType(AUDIO_STREAM_ENFORCED_AUDIBLE);
        mp->prepare();
 } else {
        ALOGE("Failed to load CameraService sounds: %s", file);
 return NULL;
 }
 return mp;
}
