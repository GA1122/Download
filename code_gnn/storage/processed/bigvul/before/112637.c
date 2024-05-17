void DocumentLoader::callContinueAfterContentPolicy(void* argument, PolicyAction policy)
{
    static_cast<DocumentLoader*>(argument)->continueAfterContentPolicy(policy);
}
