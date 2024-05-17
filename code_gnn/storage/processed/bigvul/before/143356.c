WebFrame* WebFrame::traversePrevious(bool wrap) const
{
    if (Frame* frame = toImplBase()->frame())
        return fromFrame(frame->tree().traversePreviousWithWrap(wrap));
    return 0;
}
