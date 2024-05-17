WebFrame* WebFrame::findChildByName(const WebString& name) const
{
    Frame* frame = toImplBase()->frame();
    if (!frame)
        return 0;
    return fromFrame(frame->tree().child(name));
}
