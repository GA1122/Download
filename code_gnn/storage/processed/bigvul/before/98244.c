PassRefPtr<WebFrame> WebFrame::create()
{
    RefPtr<WebFrame> frame = adoptRef(new WebFrame);

    frame->ref();

    return frame.release();
}
