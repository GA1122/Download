String HTMLElement::innerHTML() const
{
    return createMarkup(this, ChildrenOnly);
}
