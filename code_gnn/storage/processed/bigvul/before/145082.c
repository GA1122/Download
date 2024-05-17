TestingPlatformSupport::TestingPlatformSupport(const Config& config)
    : m_config(config)
    , m_oldPlatform(Platform::current())
{
    Platform::initialize(this);
}
