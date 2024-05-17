Document& Document::axObjectCacheOwner() const
{
    Document* doc = const_cast<Document*>(this);
    if (doc->frame() && doc->frame()->pagePopupOwner()) {
        DCHECK(!doc->m_axObjectCache);
        return doc->frame()->pagePopupOwner()->document().axObjectCacheOwner();
    }
    return *doc;
}
