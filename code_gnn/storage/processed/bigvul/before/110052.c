void HTMLSelectElement::scrollToSelection()
{
    if (usesMenuList())
        return;

    if (RenderObject* renderer = this->renderer())
        toRenderListBox(renderer)->selectionChanged();
}
