void InjectedBundlePage::dumpBackForwardList(StringBuilder& stringBuilder)
{
    stringBuilder.appendLiteral("\n============== Back Forward List ==============\n");

    WKBundleBackForwardListRef list = WKBundlePageGetBackForwardList(m_page);

    Vector<WKRetainPtr<WKBundleBackForwardListItemRef> > itemsToPrint;
    for (unsigned i = WKBundleBackForwardListGetForwardListCount(list); i; --i) {
        WKRetainPtr<WKBundleBackForwardListItemRef> item = adoptWK(WKBundleBackForwardListCopyItemAtIndex(list, i));
        ASSERT(!WKBundleBackForwardListItemIsSame(item.get(), m_previousTestBackForwardListItem.get()));
        itemsToPrint.append(item);
    }

    ASSERT(!WKBundleBackForwardListItemIsSame(adoptWK(WKBundleBackForwardListCopyItemAtIndex(list, 0)).get(), m_previousTestBackForwardListItem.get()));

    itemsToPrint.append(adoptWK(WKBundleBackForwardListCopyItemAtIndex(list, 0)));

    int currentItemIndex = itemsToPrint.size() - 1;

    int backListCount = WKBundleBackForwardListGetBackListCount(list);
    for (int i = -1; i >= -backListCount; --i) {
        WKRetainPtr<WKBundleBackForwardListItemRef> item = adoptWK(WKBundleBackForwardListCopyItemAtIndex(list, i));
        if (WKBundleBackForwardListItemIsSame(item.get(), m_previousTestBackForwardListItem.get()))
            break;
        itemsToPrint.append(item);
    }

    for (int i = itemsToPrint.size() - 1; i >= 0; i--)
        dumpBackForwardListItem(itemsToPrint[i].get(), 8, i == currentItemIndex, stringBuilder);

    stringBuilder.appendLiteral("===============================================\n");
}
