void FrameLoader::setOpener(LocalFrame* opener)
{
    if (client())
        client()->setOpener(opener);
}
