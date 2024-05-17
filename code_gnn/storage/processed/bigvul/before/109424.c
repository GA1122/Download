 PassRefPtr<Blob> Blob::slice(long long start, long long end, const String& contentType) const
{
    long long size;
    double modificationTime;
    if (isFile()) {
        toFile(this)->captureSnapshot(size, modificationTime);
    } else {
        ASSERT(m_size != -1);
        size = m_size;
    }

    if (start < 0)
        start = start + size;
    if (end < 0)
        end = end + size;

    if (start < 0)
        start = 0;
    if (end < 0)
        end = 0;
    if (start >= size) {
        start = 0;
        end = 0;
    } else if (end < start)
        end = start;
    else if (end > size)
        end = size;

    long long length = end - start;
    OwnPtr<BlobData> blobData = BlobData::create();
    blobData->setContentType(contentType);
    if (isFile()) {
        if (!toFile(this)->fileSystemURL().isEmpty())
            blobData->appendURL(toFile(this)->fileSystemURL(), start, length, modificationTime);
        else
            blobData->appendFile(toFile(this)->path(), start, length, modificationTime);
    } else
        blobData->appendBlob(m_internalURL, start, length);

    return Blob::create(blobData.release(), length);
}
