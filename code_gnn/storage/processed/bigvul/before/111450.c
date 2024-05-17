static VirtualKeyboardType convertInputTypeToVKBType(BlackBerryInputType inputType)
{
    switch (inputType) {
    case InputTypeURL:
        return VKBTypeUrl;
    case InputTypeEmail:
        return VKBTypeEmail;
    case InputTypeTelephone:
        return VKBTypePhone;
    case InputTypePassword:
        return VKBTypePassword;
    case InputTypeNumber:
    case InputTypeHexadecimal:
        return VKBTypePin;
    default:
        return VKBTypeDefault;
    }
}
