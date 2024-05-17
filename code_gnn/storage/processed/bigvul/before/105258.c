String HTMLElement::outerHTML() const
{
    return createMarkup(this);
}
