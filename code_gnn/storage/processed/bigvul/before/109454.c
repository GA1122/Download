void BlobData::appendFile(const String& path)
{
    m_items.append(BlobDataItem(path));
}
