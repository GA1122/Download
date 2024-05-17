void shutdownWithoutV8()
{
    ASSERT(!s_endOfTaskRunner);
    CoreInitializer::shutdown();
    Heap::shutdown();
    WTF::shutdown();
    Platform::shutdown();
    WebPrerenderingSupport::shutdown();
}
