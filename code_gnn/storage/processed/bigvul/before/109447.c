void FileReaderLoader::setEncoding(const String& encoding)
{
    if (!encoding.isEmpty())
        m_encoding = WTF::TextEncoding(encoding);
}
