Blob* XMLHttpRequest::responseBlob()
{
    ASSERT(m_responseTypeCode == ResponseTypeBlob);

    if (m_error || m_state != DONE)
        return 0;

    if (!m_responseBlob) {
        OwnPtr<BlobData> blobData = BlobData::create();
        size_t size = 0;
        if (m_binaryResponseBuilder) {
            RefPtr<RawData> rawData = RawData::create();
            size = m_binaryResponseBuilder->size();
            rawData->mutableData()->append(m_binaryResponseBuilder->data(), size);
            blobData->appendData(rawData, 0, BlobDataItem::toEndOfFile);
            blobData->setContentType(responseMIMEType());  
            m_binaryResponseBuilder.clear();
        }
        m_responseBlob = Blob::create(blobData.release(), size);
    }

    return m_responseBlob.get();
}
