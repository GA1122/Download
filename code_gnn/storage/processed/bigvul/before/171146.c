sp<IMediaMetadataRetriever> MediaPlayerService::createMetadataRetriever()
{
 pid_t pid = IPCThreadState::self()->getCallingPid();
    sp<MetadataRetrieverClient> retriever = new MetadataRetrieverClient(pid);
    ALOGV("Create new media retriever from pid %d", pid);
 return retriever;
}
