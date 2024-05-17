void FileReaderLoader::convertToDataURL()
{
    StringBuilder builder;
    builder.append("data:");

    if (!m_bytesLoaded) {
        m_stringResult = builder.toString();
        return;
    }

    builder.append(m_dataType);
    builder.append(";base64,");

    Vector<char> out;
    base64Encode(static_cast<const char*>(m_rawData->data()), m_bytesLoaded, out);
    out.append('\0');
    builder.append(out.data());

    m_stringResult = builder.toString();
}
