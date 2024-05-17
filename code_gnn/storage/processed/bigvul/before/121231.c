void HTMLInputElement::reset()
{
    if (m_inputType->storesValueSeparateFromAttribute())
        setValue(String());

    setAutofilled(false);
    setChecked(hasAttribute(checkedAttr));
    m_reflectsCheckedAttribute = true;
}
