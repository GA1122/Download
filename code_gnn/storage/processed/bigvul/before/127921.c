DocumentThreadableLoader::~DocumentThreadableLoader()
{
    m_client = nullptr;

    clearResource();
}
