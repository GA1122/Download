void FrameLoader::didAccessInitialDocumentTimerFired(Timer<FrameLoader>*)
{
    if (client())
        client()->didAccessInitialDocument();
}
