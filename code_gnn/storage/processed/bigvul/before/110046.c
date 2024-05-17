void HTMLSelectElement::remove(int optionIndex)
{
    int listIndex = optionToListIndex(optionIndex);
    if (listIndex < 0)
        return;

    listItems()[listIndex]->remove(IGNORE_EXCEPTION);
}
