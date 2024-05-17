bool Frame::isMainFrame() const
{
    return !tree().parent();
}
