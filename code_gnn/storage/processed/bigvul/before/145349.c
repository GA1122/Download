bool HTMLAnchorElement::supportsFocus() const
{
    if (hasEditableStyle())
        return HTMLElement::supportsFocus();
    return isLink() || HTMLElement::supportsFocus();
}
