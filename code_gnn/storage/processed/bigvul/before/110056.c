int HTMLSelectElement::selectedIndex() const
{
    unsigned index = 0;

    const Vector<HTMLElement*>& items = listItems();
    for (size_t i = 0; i < items.size(); ++i) {
        HTMLElement* element = items[i];
        if (element->hasTagName(optionTag)) {
            if (toHTMLOptionElement(element)->selected())
                return index;
            ++index;
        }
    }

    return -1;
}
