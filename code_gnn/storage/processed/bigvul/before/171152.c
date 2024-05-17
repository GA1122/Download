status_t MediaPlayerService::Client::dump(int fd, const Vector<String16>& args)
{
 const size_t SIZE = 256;
 char buffer[SIZE];
 String8 result;
    result.append(" Client\n");
    snprintf(buffer, 255, "  pid(%d), connId(%d), status(%d), looping(%s)\n",
            mPid, mConnId, mStatus, mLoop?"true": "false");
    result.append(buffer);
    write(fd, result.string(), result.size());
 if (mPlayer != NULL) {
        mPlayer->dump(fd, args);
 }
 if (mAudioOutput != 0) {
        mAudioOutput->dump(fd, args);
 }
    write(fd, "\n", 1);
 return NO_ERROR;
}
