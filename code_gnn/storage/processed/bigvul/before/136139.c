Document& Document::axObjectCacheOwner() const
{
    Document& top = topDocument();
    if (top.frame() && top.frame()->pagePopupOwner()) {
        ASSERT(!top.m_axObjectCache);
        return top.frame()->pagePopupOwner()->document().axObjectCacheOwner();
    }
    return top;
}
