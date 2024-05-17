int HTMLSelectElement::lastSelectedListIndex() const
{
    const Vector<HTMLElement*>& items = listItems();
    for (size_t i = items.size(); i;) {
        HTMLElement* element = items[--i];
        if (element->hasTagName(optionTag) && toHTMLOptionElement(element)->selected())
            return i;
    }
    return -1;
}
