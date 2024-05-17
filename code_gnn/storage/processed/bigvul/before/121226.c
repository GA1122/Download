inline void HTMLInputElement::removeFromRadioButtonGroup()
{
    if (CheckedRadioButtons* buttons = checkedRadioButtons())
        buttons->removeButton(this);
}
