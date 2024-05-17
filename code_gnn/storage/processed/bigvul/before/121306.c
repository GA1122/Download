bool HTMLInputElement::willRespondToMouseClickEvents()
{
    if (!isDisabledFormControl())
        return true;

    return HTMLTextFormControlElement::willRespondToMouseClickEvents();
}
