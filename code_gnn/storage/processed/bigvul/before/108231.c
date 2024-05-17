bool FrameLoader::canAuthenticateAgainstProtectionSpace(ResourceLoader* loader, const ProtectionSpace& protectionSpace)
{
    return m_client->canAuthenticateAgainstProtectionSpace(loader->documentLoader(), loader->identifier(), protectionSpace);
}
