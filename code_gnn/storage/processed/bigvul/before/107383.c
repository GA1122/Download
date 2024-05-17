AXObjectCache* Scrollbar::axObjectCache() const
{
    if (!parent() || !parent()->isFrameView())
        return 0;
    
    Document* document = static_cast<FrameView*>(parent())->frame()->document();
    return document->axObjectCache();
}
