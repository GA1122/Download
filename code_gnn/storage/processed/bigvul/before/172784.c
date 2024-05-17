MetadataRetrieverClient::MetadataRetrieverClient(pid_t pid)
{
    ALOGV("MetadataRetrieverClient constructor pid(%d)", pid);
    mPid = pid;
    mThumbnail = NULL;
    mAlbumArt = NULL;
    mRetriever = NULL;
}
