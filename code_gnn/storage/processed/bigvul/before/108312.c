void MainResourceLoader::receivedError(const ResourceError& error)
{
    RefPtr<MainResourceLoader> protect(this);
    RefPtr<Frame> protectFrame(m_frame);

    frameLoader()->receivedMainResourceError(error, true);

    if (!cancelled()) {
        ASSERT(!reachedTerminalState());
        frameLoader()->notifier()->didFailToLoad(this, error);
        
        releaseResources();
    }

    ASSERT(reachedTerminalState());
}
