void InjectedBundlePage::dumpAllFrameScrollPositions(StringBuilder& stringBuilder)
{
    WKBundleFrameRef frame = WKBundlePageGetMainFrame(m_page);
    dumpFrameScrollPosition(frame, stringBuilder);
    dumpDescendantFrameScrollPositions(frame, stringBuilder);
}
