status_t MediaPlayerService::Client::getParameter(int key, Parcel *reply) {
    ALOGV("[%d] getParameter(%d)", mConnId, key);
    sp<MediaPlayerBase> p = getPlayer();
 if (p == 0) return UNKNOWN_ERROR;
 return p->getParameter(key, reply);
}
