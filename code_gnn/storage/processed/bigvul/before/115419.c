void InjectedBundlePage::dumpAllFramesText(StringBuilder& stringBuilder)
{
    WKBundleFrameRef frame = WKBundlePageGetMainFrame(m_page);
    dumpFrameText(frame, stringBuilder);
    dumpDescendantFramesText(frame, stringBuilder);
}
