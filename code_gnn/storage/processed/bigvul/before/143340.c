 bool WebFrame::isLoading() const
 {
     if (Frame* frame = toImplBase()->frame())
        return frame->isLoading();
    return false;
}
