static void dumpBackForwardListItem(WKBundleBackForwardListItemRef item, unsigned indent, bool isCurrentItem, StringBuilder& stringBuilder)
{
    unsigned column = 0;
    if (isCurrentItem) {
        stringBuilder.appendLiteral("curr->");
        column = 6;
    }
    for (unsigned i = column; i < indent; i++)
        stringBuilder.append(' ');

    WTF::String url = toWTFString(adoptWK(WKURLCopyString(adoptWK(WKBundleBackForwardListItemCopyURL(item)).get())));
    if (hasPrefix(url, "file:")) {
        WTF::String directoryName = "/LayoutTests/";
        size_t start = url.find(directoryName);
        if (start == WTF::notFound)
            start = 0;
        else
            start += directoryName.length();
        stringBuilder.appendLiteral("(file test):");
        stringBuilder.append(url.substring(start));
    } else
        stringBuilder.append(url);

    WTF::String target = toWTFString(adoptWK(WKBundleBackForwardListItemCopyTarget(item)));
    if (target.length()) {
        stringBuilder.appendLiteral(" (in frame \"");
        stringBuilder.append(target);
        stringBuilder.appendLiteral("\")");
    }

    if (WKBundleBackForwardListItemIsTargetItem(item))
        stringBuilder.appendLiteral("  **nav target**");

    stringBuilder.append('\n');

    if (WKRetainPtr<WKArrayRef> kids = adoptWK(WKBundleBackForwardListItemCopyChildren(item))) {
        size_t size = WKArrayGetSize(kids.get());
        Vector<WKBundleBackForwardListItemRef> sortedKids(size);
        for (size_t i = 0; i < size; ++i)
            sortedKids[i] = static_cast<WKBundleBackForwardListItemRef>(WKArrayGetItemAtIndex(kids.get(), i));
        stable_sort(sortedKids.begin(), sortedKids.end(), compareByTargetName);
        for (size_t i = 0; i < size; ++i)
            dumpBackForwardListItem(sortedKids[i], indent + 4, false, stringBuilder);
    }
}
