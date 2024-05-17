void FileReaderLoader::setRange(unsigned start, unsigned length)
{
    ASSERT(length > 0);
    m_hasRange = true;
    m_rangeStart = start;
    m_rangeEnd = start + length - 1;
}
