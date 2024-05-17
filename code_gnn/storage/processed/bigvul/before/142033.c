 void TextTrackLoader::dataReceived(Resource* resource, const char* data, size_t length)
 {
     DCHECK_EQ(this->resource(), resource);

    if (m_state == Failed)
        return;

    if (!m_cueParser)
        m_cueParser = VTTParser::create(this, document());

    m_cueParser->parseBytes(data, length);
}
