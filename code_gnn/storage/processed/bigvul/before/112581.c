void Document::storageBlockingStateDidChange()
{
    if (Settings* settings = this->settings())
        securityOrigin()->setStorageBlockingPolicy(settings->storageBlockingPolicy());
}
