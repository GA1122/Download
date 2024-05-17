status_t StreamingProcessor::dump(int fd, const Vector<String16>&  ) {
 String8 result;

    result.append("  Current requests:\n");
 if (mPreviewRequest.entryCount() != 0) {
        result.append("    Preview request:\n");
        write(fd, result.string(), result.size());
        mPreviewRequest.dump(fd, 2, 6);
        result.clear();
 } else {
        result.append("    Preview request: undefined\n");
 }

 if (mRecordingRequest.entryCount() != 0) {
        result = "    Recording request:\n";
        write(fd, result.string(), result.size());
        mRecordingRequest.dump(fd, 2, 6);
        result.clear();
 } else {
        result = "    Recording request: undefined\n";
 }

 const char* streamTypeString[] = {
 "none", "preview", "record"
 };
    result.append(String8::format("   Active request: %s (paused: %s)\n",
                                  streamTypeString[mActiveRequest],
                                  mPaused ? "yes" : "no"));

    write(fd, result.string(), result.size());

 return OK;
}
