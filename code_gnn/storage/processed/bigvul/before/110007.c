void HTMLSelectElement::deselectItems(HTMLOptionElement* excludeElement)
{
    deselectItemsWithoutValidation(excludeElement);
    setNeedsValidityCheck();
}
