bool HTMLAnchorElement::isMouseFocusable() const
{
    if (isLink())
        return supportsFocus();

    return HTMLElement::isMouseFocusable();
}
