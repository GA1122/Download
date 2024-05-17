String ShadowRoot::innerHTML() const
{
    return createMarkup(this, ChildrenOnly);
}
