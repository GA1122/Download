Document& Document::axObjectCacheOwner() const
{
    Document* top = const_cast<Document*>(this);
    LocalFrame* frame = this->frame();
    if (!frame)
        return *top;

    while (frame && frame->owner() && frame->owner()->isLocal()) {
        HTMLFrameOwnerElement* owner = toHTMLFrameOwnerElement(frame->owner());
        top = &owner->document();
        frame = top->frame();
    }

    if (top->frame() && top->frame()->pagePopupOwner()) {
        ASSERT(!top->m_axObjectCache);
        return top->frame()->pagePopupOwner()->document().axObjectCacheOwner();
    }

    ASSERT(top);
    return *top;
}
