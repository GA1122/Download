bool MediaPlayerService::Client::shouldDropMetadata(media::Metadata::Type code) const
{
 Mutex::Autolock lock(mLock);

 if (findMetadata(mMetadataDrop, code)) {
 return true;
 }

 if (mMetadataAllow.isEmpty() || findMetadata(mMetadataAllow, code)) {
 return false;
 } else {
 return true;
 }
}
