void MediaPlayerService::Client::addNewMetadataUpdate(media::Metadata::Type metadata_type) {
 Mutex::Autolock lock(mLock);
 if (mMetadataUpdated.indexOf(metadata_type) < 0) {
        mMetadataUpdated.add(metadata_type);
 }
}
