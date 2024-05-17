Element* Document::activeElement() const
{
    if (Element* element = adjustedFocusedElement())
        return element;
    return body();
}
