bool HTMLSelectElement::hasPlaceholderLabelOption() const
{
    if (multiple() || size() > 1)
        return false;

    int listIndex = optionToListIndex(0);
    ASSERT(listIndex >= 0);
    if (listIndex < 0)
        return false;
    return !listIndex && toHTMLOptionElement(listItems()[listIndex])->value().isEmpty();
}
