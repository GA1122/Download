Frame* FrameLoader::opener()
{
    return client() ? client()->opener() : 0;
}
