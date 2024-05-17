static VirtualKeyboardEnterKeyType convertStringToKeyboardEnterKeyType(const AtomicString& string)
{
    DEFINE_STATIC_LOCAL(AtomicString, Default, ("default"));
    DEFINE_STATIC_LOCAL(AtomicString, Connect, ("connect"));
    DEFINE_STATIC_LOCAL(AtomicString, Done, ("done"));
    DEFINE_STATIC_LOCAL(AtomicString, Go, ("go"));
    DEFINE_STATIC_LOCAL(AtomicString, Join, ("join"));
    DEFINE_STATIC_LOCAL(AtomicString, Next, ("next"));
    DEFINE_STATIC_LOCAL(AtomicString, Search, ("search"));
    DEFINE_STATIC_LOCAL(AtomicString, Send, ("send"));
    DEFINE_STATIC_LOCAL(AtomicString, Submit, ("submit"));

    if (string.isEmpty())
        return VKBEnterKeyNotSet;
    if (equalIgnoringCase(string, Default))
        return VKBEnterKeyDefault;
    if (equalIgnoringCase(string, Connect))
        return VKBEnterKeyConnect;
    if (equalIgnoringCase(string, Done))
        return VKBEnterKeyDone;
    if (equalIgnoringCase(string, Go))
        return VKBEnterKeyGo;
    if (equalIgnoringCase(string, Join))
        return VKBEnterKeyJoin;
    if (equalIgnoringCase(string, Next))
        return VKBEnterKeyNext;
    if (equalIgnoringCase(string, Search))
        return VKBEnterKeySearch;
    if (equalIgnoringCase(string, Send))
        return VKBEnterKeySend;
    if (equalIgnoringCase(string, Submit))
        return VKBEnterKeySubmit;
    return VKBEnterKeyNotSet;
}
