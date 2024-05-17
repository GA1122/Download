String HTMLSelectElement::value() const
{
    const Vector<HTMLElement*>& items = listItems();
    for (unsigned i = 0; i < items.size(); i++) {
        if (items[i]->hasLocalName(optionTag) && toHTMLOptionElement(items[i])->selected())
            return toHTMLOptionElement(items[i])->value();
    }
    return "";
}
