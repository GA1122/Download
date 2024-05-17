static BlackBerryInputType convertInputType(const HTMLInputElement* inputElement)
{
    if (inputElement->isPasswordField())
        return InputTypePassword;
    if (inputElement->isSearchField())
        return InputTypeSearch;
    if (inputElement->isEmailField())
        return InputTypeEmail;
    if (inputElement->isMonthControl())
        return InputTypeMonth;
    if (inputElement->isNumberField())
        return InputTypeNumber;
    if (inputElement->isTelephoneField())
        return InputTypeTelephone;
    if (inputElement->isURLField())
        return InputTypeURL;
#if ENABLE(INPUT_TYPE_COLOR)
    if (inputElement->isColorControl())
        return InputTypeColor;
#endif
    if (inputElement->isDateControl())
        return InputTypeDate;
    if (inputElement->isDateTimeControl())
        return InputTypeDateTime;
    if (inputElement->isDateTimeLocalControl())
        return InputTypeDateTimeLocal;
    if (inputElement->isTimeControl())
        return InputTypeTime;
    if (DOMSupport::elementIdOrNameIndicatesEmail(inputElement))
        return InputTypeEmail;
    if (DOMSupport::elementIdOrNameIndicatesUrl(inputElement))
        return InputTypeURL;
    if (DOMSupport::elementPatternIndicatesNumber(inputElement))
        return InputTypeNumber;
    if (DOMSupport::elementPatternIndicatesHexadecimal(inputElement))
        return InputTypeHexadecimal;

    return InputTypeText;
}
