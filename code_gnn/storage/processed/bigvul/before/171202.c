status_t MediaPlayerService::Client::setDataSource(
 const sp<IDataSource> &source) {
    sp<DataSource> dataSource = DataSource::CreateFromIDataSource(source);
    player_type playerType = MediaPlayerFactory::getPlayerType(this, dataSource);
    sp<MediaPlayerBase> p = setDataSource_pre(playerType);
 if (p == NULL) {
 return NO_INIT;
 }
    setDataSource_post(p, p->setDataSource(dataSource));
 return mStatus;
}
