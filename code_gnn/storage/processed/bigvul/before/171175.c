void MediaPlayerService::instantiate() {
    defaultServiceManager()->addService(
 String16("media.player"), new MediaPlayerService());
}
