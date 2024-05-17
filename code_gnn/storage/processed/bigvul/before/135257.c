bool Document::hasFocus() const
{
    return page() && page()->focusController().isDocumentFocused(*this);
}
