void NuPlayer::GenericSource::ensureCacheIsFetching() {
 if (mCachedSource != NULL) {
        mCachedSource->resumeFetchingIfNecessary();
 }
}
