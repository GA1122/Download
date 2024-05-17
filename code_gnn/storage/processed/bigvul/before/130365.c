void HTMLFormControlElement::setFocus(bool flag)
{
    LabelableElement::setFocus(flag);

    if (!flag && wasChangedSinceLastFormControlChangeEvent())
        dispatchFormControlChangeEvent();
}
