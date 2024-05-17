bool Frame::isLocalRoot() const
{
    if (isRemoteFrame())
        return false;

    if (!tree().parent())
        return true;

    return tree().parent()->isRemoteFrame();
}
