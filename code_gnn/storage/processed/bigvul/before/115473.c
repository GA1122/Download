static JSRetainPtr<JSStringRef> toJS(const char* string)
{
    return JSRetainPtr<JSStringRef>(Adopt, JSStringCreateWithUTF8CString(string));
}
