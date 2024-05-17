static JSValueRef keyDownCallback(JSContextRef context, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
{
    if (argumentCount < 1)
        return JSValueMakeUndefined(context);

    JSStringRef character = JSValueToStringCopy(context, arguments[0], exception);
    ASSERT(!*exception);
    short charCode = 0;
    bool needsShiftKeyModifier = false;
    if (JSStringIsEqualToUTF8CString(character, "leftArrow"))
        charCode = KEYCODE_LEFT;
    else if (JSStringIsEqualToUTF8CString(character, "rightArrow"))
        charCode = KEYCODE_RIGHT;
    else if (JSStringIsEqualToUTF8CString(character, "upArrow"))
        charCode = KEYCODE_UP;
    else if (JSStringIsEqualToUTF8CString(character, "downArrow"))
        charCode = KEYCODE_DOWN;
    else if (JSStringIsEqualToUTF8CString(character, "pageUp"))
        charCode = KEYCODE_PG_UP;
    else if (JSStringIsEqualToUTF8CString(character, "pageDown"))
        charCode = KEYCODE_PG_DOWN;
    else if (JSStringIsEqualToUTF8CString(character, "home"))
        charCode = KEYCODE_HOME;
    else if (JSStringIsEqualToUTF8CString(character, "end"))
        charCode = KEYCODE_END;
    else if (JSStringIsEqualToUTF8CString(character, "delete"))
        charCode = KEYCODE_BACKSPACE;
    else {
        charCode = JSStringGetCharactersPtr(character)[0];
        if (0x8 == charCode)
            charCode = KEYCODE_BACKSPACE;
        else if (0x7F == charCode)
            charCode = KEYCODE_DELETE;
        else if (WTF::isASCIIUpper(charCode))
            needsShiftKeyModifier = true;
    }
    JSStringRelease(character);

    static const JSStringRef lengthProperty = JSStringCreateWithUTF8CString("length");
    bool needsAltKeyModifier = false;
    bool needsCtrlKeyModifier = false;
    if (argumentCount > 1) {
        if (JSObjectRef modifiersArray = JSValueToObject(context, arguments[1], 0)) {
            int modifiersCount = JSValueToNumber(context, JSObjectGetProperty(context, modifiersArray, lengthProperty, 0), 0);
            for (int i = 0; i < modifiersCount; ++i) {
                JSStringRef string = JSValueToStringCopy(context, JSObjectGetPropertyAtIndex(context, modifiersArray, i, 0), 0);
                if (JSStringIsEqualToUTF8CString(string, "shiftKey"))
                    needsShiftKeyModifier = true;
                else if (JSStringIsEqualToUTF8CString(string, "altKey"))
                    needsAltKeyModifier = true;
                else if (JSStringIsEqualToUTF8CString(string, "ctrlKey"))
                    needsCtrlKeyModifier = true;
                JSStringRelease(string);
            }
        }
    }

    BlackBerry::WebKit::WebPage* page = BlackBerry::WebKit::DumpRenderTree::currentInstance()->page();

    unsigned modifiers = 0;
    if (needsShiftKeyModifier)
        modifiers |= KEYMOD_SHIFT;
    if (needsAltKeyModifier)
        modifiers |= KEYMOD_ALT;
    if (needsCtrlKeyModifier)
        modifiers |= KEYMOD_CTRL;

    page->keyEvent(BlackBerry::Platform::KeyboardEvent(charCode, BlackBerry::Platform::KeyboardEvent::KeyDown, modifiers));
    page->keyEvent(BlackBerry::Platform::KeyboardEvent(charCode, BlackBerry::Platform::KeyboardEvent::KeyUp, modifiers));

    return JSValueMakeUndefined(context);
}
