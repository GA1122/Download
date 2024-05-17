HTMLInputElement* HTMLInputElement::checkedRadioButtonForGroup() const
{
    if (CheckedRadioButtons* buttons = checkedRadioButtons())
        return buttons->checkedButtonForGroup(name());
    return 0;
}
