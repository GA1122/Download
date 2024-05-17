FormControlState HTMLSelectElement::saveFormControlState() const
{
    const Vector<HTMLElement*>& items = listItems();
    size_t length = items.size();
    FormControlState state;
    for (unsigned i = 0; i < length; ++i) {
        if (!items[i]->hasTagName(optionTag))
            continue;
        HTMLOptionElement* option = toHTMLOptionElement(items[i]);
        if (!option->selected())
            continue;
        state.append(option->value());
        if (!multiple())
            break;
    }
    return state;
}
