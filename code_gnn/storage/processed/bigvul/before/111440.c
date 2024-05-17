void InputHandler::addAttributedTextMarker(int start, int end, const AttributeTextStyle& style)
{
    if ((end - start) < 1 || end > static_cast<int>(elementText().length()))
        return;

    RefPtr<Range> markerRange = DOMSupport::visibleSelectionForRangeInputElement(m_currentFocusElement.get(), start, end).toNormalizedRange();
    m_currentFocusElement->document()->markers()->addMarker(markerRange.get(), DocumentMarker::AttributeText, WTF::String("Input Marker"), style);
}
