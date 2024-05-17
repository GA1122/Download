String HTMLSelectElement::validationMessage() const
{
    if (!willValidate())
        return String();

    if (customError())
        return customValidationMessage();

    return valueMissing() ? validationMessageValueMissingForSelectText() : String();
}
