int HTMLSelectElement::listToOptionIndex(int listIndex) const
{
    const Vector<HTMLElement*>& items = listItems();
    if (listIndex < 0 || listIndex >= static_cast<int>(items.size()) || !items[listIndex]->hasTagName(optionTag))
        return -1;

    int optionIndex = 0;
    for (int i = 0; i < listIndex; ++i) {
        if (items[i]->hasTagName(optionTag))
            ++optionIndex;
    }

    return optionIndex;
}
