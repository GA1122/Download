status_t MetadataRetrieverClient::dump(int fd, const Vector<String16>&  )
{
 const size_t SIZE = 256;
 char buffer[SIZE];
 String8 result;
    result.append(" MetadataRetrieverClient\n");
    snprintf(buffer, 255, "  pid(%d)\n", mPid);
    result.append(buffer);
    write(fd, result.string(), result.size());
    write(fd, "\n", 1);
 return NO_ERROR;
}
