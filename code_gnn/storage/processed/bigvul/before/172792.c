status_t MetadataRetrieverClient::setDataSource(
 const sp<IDataSource>& source)
{
    ALOGV("setDataSource(IDataSource)");
 Mutex::Autolock lock(mLock);

    sp<DataSource> dataSource = DataSource::CreateFromIDataSource(source);
    player_type playerType =
 MediaPlayerFactory::getPlayerType(NULL  , dataSource);
    ALOGV("player type = %d", playerType);
    sp<MediaMetadataRetrieverBase> p = createRetriever(playerType);
 if (p == NULL) return NO_INIT;
 status_t ret = p->setDataSource(dataSource);
 if (ret == NO_ERROR) mRetriever = p;
 return ret;
}
