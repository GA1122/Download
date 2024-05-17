status_t MediaPlayerService::Client::getMetadata(
 bool update_only, bool  , Parcel *reply)
{
    sp<MediaPlayerBase> player = getPlayer();
 if (player == 0) return UNKNOWN_ERROR;

 status_t status;
    reply->writeInt32(-1);

    media::Metadata::Filter ids;

 {
 Mutex::Autolock lock(mLock);
 if (update_only) {
            ids = mMetadataUpdated;
 }
        mMetadataUpdated.clear();
 }

    media::Metadata metadata(reply);

    metadata.appendHeader();
    status = player->getMetadata(ids, reply);

 if (status != OK) {
        metadata.resetParcel();
        ALOGE("getMetadata failed %d", status);
 return status;
 }


    metadata.updateLength();
 return OK;
}
