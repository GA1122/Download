void WebView::scrollView(const IntRect& scrollRect, const IntSize& scrollOffset)
{
    setViewNeedsDisplay(scrollRect);
}
