TestingPlatformSupport::~TestingPlatformSupport()
{
    Platform::initialize(m_oldPlatform);
}
