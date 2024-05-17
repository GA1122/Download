status_t MediaPlayerService::Client::setDataSource(
 const sp<IStreamSource> &source) {
    player_type playerType = MediaPlayerFactory::getPlayerType(this, source);
    sp<MediaPlayerBase> p = setDataSource_pre(playerType);
 if (p == NULL) {
 return NO_INIT;
 }

    setDataSource_post(p, p->setDataSource(source));
 return mStatus;
}
