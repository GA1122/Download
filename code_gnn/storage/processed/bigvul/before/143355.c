WebFrame* WebFrame::traverseNext(bool wrap) const
{
    if (Frame* frame = toImplBase()->frame())
        return fromFrame(frame->tree().traverseNextWithWrap(wrap));
    return 0;
}
