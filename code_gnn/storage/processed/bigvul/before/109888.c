SandboxFlags SecurityContext::parseSandboxPolicy(const String& policy, String& invalidTokensErrorMessage)
{
    SandboxFlags flags = SandboxAll;
    unsigned length = policy.length();
    unsigned start = 0;
    unsigned numberOfTokenErrors = 0;
    StringBuilder tokenErrors;
    while (true) {
        while (start < length && isHTMLSpace<UChar>(policy[start]))
            ++start;
        if (start >= length)
            break;
        unsigned end = start + 1;
        while (end < length && !isHTMLSpace<UChar>(policy[end]))
            ++end;

        String sandboxToken = policy.substring(start, end - start);
        if (equalIgnoringCase(sandboxToken, "allow-same-origin"))
            flags &= ~SandboxOrigin;
        else if (equalIgnoringCase(sandboxToken, "allow-forms"))
            flags &= ~SandboxForms;
        else if (equalIgnoringCase(sandboxToken, "allow-scripts")) {
            flags &= ~SandboxScripts;
            flags &= ~SandboxAutomaticFeatures;
        } else if (equalIgnoringCase(sandboxToken, "allow-top-navigation"))
            flags &= ~SandboxTopNavigation;
        else if (equalIgnoringCase(sandboxToken, "allow-popups"))
            flags &= ~SandboxPopups;
        else if (equalIgnoringCase(sandboxToken, "allow-pointer-lock"))
            flags &= ~SandboxPointerLock;
        else {
            if (numberOfTokenErrors)
                tokenErrors.appendLiteral(", '");
            else
                tokenErrors.append('\'');
            tokenErrors.append(sandboxToken);
            tokenErrors.append('\'');
            numberOfTokenErrors++;
        }

        start = end + 1;
    }

    if (numberOfTokenErrors) {
        if (numberOfTokenErrors > 1)
            tokenErrors.appendLiteral(" are invalid sandbox flags.");
        else
            tokenErrors.appendLiteral(" is an invalid sandbox flag.");
        invalidTokensErrorMessage = tokenErrors.toString();
    }

    return flags;
}
