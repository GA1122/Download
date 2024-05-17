Vector<IconURL> Document::iconURLs(int iconTypesMask)
{
    IconURL firstFavicon;
    IconURL firstTouchIcon;
    IconURL firstTouchPrecomposedIcon;
    Vector<IconURL> secondaryIcons;

    for (HTMLLinkElement* linkElement = head() ? Traversal<HTMLLinkElement>::firstChild(*head()) : 0; linkElement; linkElement = Traversal<HTMLLinkElement>::nextSibling(*linkElement)) {
        if (!(linkElement->iconType() & iconTypesMask))
            continue;
        if (linkElement->href().isEmpty())
            continue;

        IconURL newURL(linkElement->href(), linkElement->iconSizes(), linkElement->type(), linkElement->iconType());
        if (linkElement->iconType() == Favicon) {
            if (firstFavicon.m_iconType != InvalidIcon)
                secondaryIcons.append(firstFavicon);
            firstFavicon = newURL;
        } else if (linkElement->iconType() == TouchIcon) {
            if (firstTouchIcon.m_iconType != InvalidIcon)
                secondaryIcons.append(firstTouchIcon);
            firstTouchIcon = newURL;
        } else if (linkElement->iconType() == TouchPrecomposedIcon) {
            if (firstTouchPrecomposedIcon.m_iconType != InvalidIcon)
                secondaryIcons.append(firstTouchPrecomposedIcon);
            firstTouchPrecomposedIcon = newURL;
        } else {
            ASSERT_NOT_REACHED();
        }
    }

    Vector<IconURL> iconURLs;
    if (firstFavicon.m_iconType != InvalidIcon)
        iconURLs.append(firstFavicon);
    else if (m_url.protocolIsInHTTPFamily() && iconTypesMask & Favicon)
        iconURLs.append(IconURL::defaultFavicon(m_url));

    if (firstTouchIcon.m_iconType != InvalidIcon)
        iconURLs.append(firstTouchIcon);
    if (firstTouchPrecomposedIcon.m_iconType != InvalidIcon)
        iconURLs.append(firstTouchPrecomposedIcon);
    for (int i = secondaryIcons.size() - 1; i >= 0; --i)
        iconURLs.append(secondaryIcons[i]);
    return iconURLs;
}
