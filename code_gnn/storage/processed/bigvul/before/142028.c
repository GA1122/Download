DEFINE_TRACE(TextTrackLoader)
{
    visitor->trace(m_client);
    visitor->trace(m_cueParser);
    visitor->trace(m_document);
    ResourceOwner<RawResource>::trace(visitor);
    VTTParserClient::trace(visitor);
}
