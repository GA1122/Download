WebFileSystem* LocalFileSystem::fileSystem() const
{
    Platform* platform = Platform::current();
    if (!platform)
        return nullptr;
    return Platform::current()->fileSystem();
}
