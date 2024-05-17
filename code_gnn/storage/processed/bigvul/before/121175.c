bool HTMLInputElement::isInRequiredRadioButtonGroup()
{
    ASSERT(isRadioButton());
    if (CheckedRadioButtons* buttons = checkedRadioButtons())
        return buttons->isInRequiredGroup(this);
    return false;
}
