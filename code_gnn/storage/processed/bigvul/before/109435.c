PassRefPtr<Blob> FileReaderLoader::blobResult()
{
    ASSERT(m_readType == ReadAsBlob);

    if (!m_rawData || m_errorCode || !isCompleted())
        return 0;

    if (!m_blobResult) {
        OwnPtr<BlobData> blobData = BlobData::create();
        size_t size = 0;
        RefPtr<RawData> rawData = RawData::create();
        size = m_rawData->byteLength();
        rawData->mutableData()->append(static_cast<char*>(m_rawData->data()), size);
        blobData->appendData(rawData, 0, size);
        blobData->setContentType(m_dataType);
        m_blobResult = Blob::create(blobData.release(), size);
    }
    return m_blobResult;
}
