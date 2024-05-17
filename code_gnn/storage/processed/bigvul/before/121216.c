static Vector<String> parseAcceptAttribute(const String& acceptString, bool (*predicate)(const String&))
{
    Vector<String> types;
    if (acceptString.isEmpty())
        return types;

    Vector<String> splitTypes;
    acceptString.split(',', false, splitTypes);
    for (size_t i = 0; i < splitTypes.size(); ++i) {
        String trimmedType = stripLeadingAndTrailingHTMLSpaces(splitTypes[i]);
        if (trimmedType.isEmpty())
            continue;
        if (!predicate(trimmedType))
            continue;
        types.append(trimmedType.lower());
    }

    return types;
}
