void InjectedBundlePage::prepare()
{
    WKBundlePageClearMainFrameName(m_page);

    WKBundlePageSetPageZoomFactor(m_page, 1);
    WKBundlePageSetTextZoomFactor(m_page, 1);

    WKPoint origin = { 0, 0 };
    WKBundlePageSetScaleAtOrigin(m_page, 1, origin);

    m_previousTestBackForwardListItem = adoptWK(WKBundleBackForwardListCopyItemAtIndex(WKBundlePageGetBackForwardList(m_page), 0));

    WKBundleFrameClearOpener(WKBundlePageGetMainFrame(m_page));
    
    WKBundlePageSetTracksRepaints(m_page, false);
}
