void TokenPreloadScanner::scan(const CompactHTMLToken& token, const SegmentedString& source, PreloadRequestStream& requests, ViewportDescriptionWrapper* viewport, bool* likelyDocumentWriteScript)
{
    scanCommon(token, source, requests, viewport, likelyDocumentWriteScript);
}
