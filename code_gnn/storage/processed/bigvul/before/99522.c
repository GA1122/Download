static NPUTF8 *NPN_UTF8FromIdentifier(NPIdentifier identifier)
{
    const char* string = static_cast<IdentifierRep*>(identifier)->string();
    if (!string)
        return 0;

    uint32_t stringLength = strlen(string);
    char* utf8String = static_cast<char*>(NPN_MemAlloc(stringLength + 1));
    memcpy(utf8String, string, stringLength);
    utf8String[stringLength] = '\0';
    
    return utf8String;
}
