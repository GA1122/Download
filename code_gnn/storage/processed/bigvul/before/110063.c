void HTMLSelectElement::setOptionsChangedOnRenderer()
{
    if (RenderObject* renderer = this->renderer()) {
        if (usesMenuList())
            toRenderMenuList(renderer)->setOptionsChanged(true);
        else
            toRenderListBox(renderer)->setOptionsChanged(true);
    }
}
