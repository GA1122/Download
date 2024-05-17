inline void HTMLInputElement::addToRadioButtonGroup()
{
    if (CheckedRadioButtons* buttons = checkedRadioButtons())
        buttons->addButton(this);
}
