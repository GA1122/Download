void TextTrackLoader::newRegionsParsed()
{
    m_client->newRegionsAvailable(this);
}
