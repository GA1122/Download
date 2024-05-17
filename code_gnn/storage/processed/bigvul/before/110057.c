PassRefPtr<HTMLCollection> HTMLSelectElement::selectedOptions()
{
    updateListItemSelectedStates();
    return ensureCachedHTMLCollection(SelectedOptions);
}
