void WebFrame::detach()
{
    toImplBase()->frame()->detach(FrameDetachType::Remove);
}
