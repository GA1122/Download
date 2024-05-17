void HTMLSelectElement::invalidateSelectedItems()
{
    if (HTMLCollection* collection = cachedHTMLCollection(SelectedOptions))
        collection->invalidateCache();
}
