bool HTMLSelectElement::appendFormData(FormDataList& list, bool)
{
    const AtomicString& name = this->name();
    if (name.isEmpty())
        return false;

    bool successful = false;
    const Vector<HTMLElement*>& items = listItems();

    for (unsigned i = 0; i < items.size(); ++i) {
        HTMLElement* element = items[i];
        if (element->hasTagName(optionTag) && toHTMLOptionElement(element)->selected() && !toHTMLOptionElement(element)->isDisabledFormControl()) {
            list.appendData(name, toHTMLOptionElement(element)->value());
            successful = true;
        }
    }

    return successful;
} 
