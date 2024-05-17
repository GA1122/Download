static const StringImpl* tagImplFor(const String& tagName)
{
    const StringImpl* result = tagName.impl();
    if (result->isStatic())
        return result;
    return nullptr;
}
