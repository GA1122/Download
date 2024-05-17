static int64_t inputStyle(BlackBerryInputType type, const Element* element)
{
    switch (type) {
    case InputTypeEmail:
    case InputTypeURL:
    case InputTypeSearch:
    case InputTypeText:
    case InputTypeTextArea:
        {
            DOMSupport::AttributeState autoCompleteState = DOMSupport::elementSupportsAutocomplete(element);
            DOMSupport::AttributeState autoCorrectState = DOMSupport::elementSupportsAutocorrect(element);

            if (autoCompleteState == DOMSupport::Off) {
                if (autoCorrectState == DOMSupport::On)
                    return NO_PREDICTION;
                return NO_AUTO_TEXT | NO_PREDICTION | NO_AUTO_CORRECTION;
            }

            if (autoCompleteState == DOMSupport::On) {
                if (autoCorrectState == DOMSupport::Off)
                    return NO_AUTO_TEXT | NO_AUTO_CORRECTION;
                return DEFAULT_STYLE;
            }

            if (type == InputTypeEmail || type == InputTypeURL || DOMSupport::elementIdOrNameIndicatesNoAutocomplete(element)) {
                if (autoCorrectState == DOMSupport::On)
                    return NO_PREDICTION;
                return NO_AUTO_TEXT | NO_PREDICTION | NO_AUTO_CORRECTION;
            }

            if (autoCorrectState == DOMSupport::On || (type == InputTypeTextArea && autoCorrectState != DOMSupport::Off))
                return DEFAULT_STYLE;

            return NO_AUTO_TEXT | NO_AUTO_CORRECTION;
        }
    case InputTypePassword:
    case InputTypeNumber:
    case InputTypeTelephone:
    case InputTypeHexadecimal:
        return NO_AUTO_TEXT | NO_PREDICTION | NO_AUTO_CORRECTION;
    default:
        break;
    }
    return DEFAULT_STYLE;
}
