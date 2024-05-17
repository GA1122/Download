static int adjustForZoom(int value, Document* document)
{
    LocalFrame* frame = document->frame();
    float zoomFactor = frame->pageZoomFactor();
    if (zoomFactor == 1)
        return value;
    if (zoomFactor > 1)
        value++;
    return static_cast<int>(value / zoomFactor);
}
