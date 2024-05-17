void HTMLFormControlElement::requiredAttributeChanged()
{
    setNeedsValidityCheck();
    pseudoStateChanged(CSSSelector::PseudoRequired);
    pseudoStateChanged(CSSSelector::PseudoOptional);
}
