void MainResourceLoader::continueAfterContentPolicy(PolicyAction policy)
{
    ASSERT(m_waitingForContentPolicy);
    m_waitingForContentPolicy = false;
    if (frameLoader() && !frameLoader()->isStopping())
        continueAfterContentPolicy(policy, m_response);
    deref();  
}
