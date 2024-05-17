void HTMLSelectElement::setValue(const String &value)
{
    if (value.isNull()) {
        setSelectedIndex(-1);
        return;
    }

    const Vector<HTMLElement*>& items = listItems();
    unsigned optionIndex = 0;
    for (unsigned i = 0; i < items.size(); i++) {
        if (items[i]->hasLocalName(optionTag)) {
            if (toHTMLOptionElement(items[i])->value() == value) {
                setSelectedIndex(optionIndex);
                return;
            }
            optionIndex++;
        }
    }

    setSelectedIndex(-1);
}
