bool Document::isInMainFrame() const
{
    return frame() && frame()->isMainFrame();
}
