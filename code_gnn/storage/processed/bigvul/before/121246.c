void HTMLInputElement::setEditingValue(const String& value)
{
    if (!renderer() || !isTextField())
        return;
    setInnerTextValue(value);
    subtreeHasChanged();

    unsigned max = value.length();
    if (focused())
        setSelectionRange(max, max);
    else
        cacheSelectionInResponseToSetValue(max);

    dispatchInputEvent();
}
