unsigned HTMLSelectElement::length() const
{
    unsigned options = 0;

    const Vector<HTMLElement*>& items = listItems();
    for (unsigned i = 0; i < items.size(); ++i) {
        if (items[i]->hasTagName(optionTag))
            ++options;
    }

    return options;
}
