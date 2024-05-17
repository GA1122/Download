void HTMLSelectElement::deselectItemsWithoutValidation(HTMLElement* excludeElement)
{
    const Vector<HTMLElement*>& items = listItems();
    for (unsigned i = 0; i < items.size(); ++i) {
        HTMLElement* element = items[i];
        if (element != excludeElement && element->hasTagName(optionTag))
            toHTMLOptionElement(element)->setSelectedState(false);
    }
}
