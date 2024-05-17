static bool acceptsEditingFocus(const Element& element)
{
    DCHECK(element.hasEditableStyle());

    return element.document().frame() && element.rootEditableElement();
}
