void BlobData::appendData(PassRefPtr<RawData> data, long long offset, long long length)
{
    m_items.append(BlobDataItem(data, offset, length));
}
