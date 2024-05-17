static NPError parsePostBuffer(bool isFile, const char *buffer, uint32_t length, bool parseHeaders, HTTPHeaderMap& headerFields, Vector<char>& bodyData)
{
    RefPtr<SharedBuffer> fileContents;
    const char* postBuffer = 0;
    uint32_t postBufferSize = 0;

    if (isFile) {
        fileContents = SharedBuffer::createWithContentsOfFile(String::fromUTF8(buffer));
        if (!fileContents)
            return NPERR_FILE_NOT_FOUND;

        postBuffer = fileContents->data();
        postBufferSize = fileContents->size();

    } else {
        postBuffer = buffer;
        postBufferSize = length;
    }

    if (parseHeaders) {
        if (startsWithBlankLine(postBuffer, postBufferSize)) {
            postBuffer++;
            postBufferSize--;
        } else {
            int location = locationAfterFirstBlankLine(postBuffer, postBufferSize);
            if (location != -1) {
                headerFields = parseRFC822HeaderFields(postBuffer, location);
                unsigned dataLength = postBufferSize - location;
                
                String contentLength = headerFields.get("Content-Length");
                
                if (!contentLength.isNull())
                    dataLength = min(contentLength.toInt(), (int)dataLength);
                headerFields.remove("Content-Length");
                
                postBuffer += location;
                postBufferSize = dataLength;
                
            }
        }
    }

    ASSERT(bodyData.isEmpty());
    bodyData.append(postBuffer, postBufferSize);

    return NPERR_NO_ERROR;
}
