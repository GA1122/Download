static bool prepareResourceBuffer(Resource* cachedResource, bool* hasZeroSize)
{
    *hasZeroSize = false;
    if (!cachedResource)
        return false;

    if (cachedResource->dataBufferingPolicy() == DoNotBufferData)
        return false;

    if (!cachedResource->encodedSize()) {
        *hasZeroSize = true;
        return true;
    }

    if (cachedResource->isPurgeable()) {
        if (!cachedResource->makePurgeable(false))
            return false;
    }

    return true;
}
