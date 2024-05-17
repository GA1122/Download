static WKRetainPtr<WKStringRef> NavigationTypeToString(WKFrameNavigationType type)
{
    switch (type) {
    case kWKFrameNavigationTypeLinkClicked:
        return adoptWK(WKStringCreateWithUTF8CString("link clicked"));
    case kWKFrameNavigationTypeFormSubmitted:
        return adoptWK(WKStringCreateWithUTF8CString("form submitted"));
    case kWKFrameNavigationTypeBackForward:
        return adoptWK(WKStringCreateWithUTF8CString("back/forward"));
    case kWKFrameNavigationTypeReload:
        return adoptWK(WKStringCreateWithUTF8CString("reload"));
    case kWKFrameNavigationTypeFormResubmitted:
        return adoptWK(WKStringCreateWithUTF8CString("form resubmitted"));
    case kWKFrameNavigationTypeOther:
        return adoptWK(WKStringCreateWithUTF8CString("other"));
    }
    return adoptWK(WKStringCreateWithUTF8CString("illegal value"));
}
