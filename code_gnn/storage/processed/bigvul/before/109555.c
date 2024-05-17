static bool acceptsEditingFocus(Element* element)
{
    ASSERT(element);
    ASSERT(element->rendererIsEditable());

    return element->document().frame() && element->rootEditableElement();
}
