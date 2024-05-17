sp<MediaPlayerBase> MediaPlayerService::Client::createPlayer(player_type playerType)
{
    sp<MediaPlayerBase> p = mPlayer;
 if ((p != NULL) && (p->playerType() != playerType)) {
        ALOGV("delete player");
        p.clear();
 }
 if (p == NULL) {
        p = MediaPlayerFactory::createPlayer(playerType, this, notify, mPid);
 }

 if (p != NULL) {
        p->setUID(mUID);
 }

 return p;
}
