static bool acceptsEditingFocus(const Element& element)
{
    ASSERT(element.hasEditableStyle());

    return element.document().frame() && element.rootEditableElement();
}
