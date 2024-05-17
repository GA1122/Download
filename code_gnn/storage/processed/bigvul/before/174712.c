MediaMetadataRetriever::~MediaMetadataRetriever()
{
    ALOGV("destructor");
    disconnect();
 IPCThreadState::self()->flushCommands();
}
