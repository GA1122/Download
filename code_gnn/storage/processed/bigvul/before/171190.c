void MediaPlayerService::removeMediaRecorderClient(wp<MediaRecorderClient> client)
{
 Mutex::Autolock lock(mLock);
    mMediaRecorderClients.remove(client);
    ALOGV("Delete media recorder client");
}
