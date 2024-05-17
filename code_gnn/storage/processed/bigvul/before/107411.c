Scrollbar::~Scrollbar()
{
    if (AXObjectCache::accessibilityEnabled() && axObjectCache())
        axObjectCache()->remove(this);
    
    stopTimerIfNeeded();
    
    m_theme->unregisterScrollbar(this);
}
