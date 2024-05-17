bool WebPage::executeJavaScriptInIsolatedWorld(const BlackBerry::Platform::String& scriptUTF8, JavaScriptDataType& returnType, BlackBerry::Platform::String& returnValue)
{
    BLACKBERRY_ASSERT(scriptUTF8.isUtf8());
    ScriptSourceCode sourceCode(scriptUTF8, KURL());
    return d->executeJavaScriptInIsolatedWorld(sourceCode, returnType, returnValue);
}
