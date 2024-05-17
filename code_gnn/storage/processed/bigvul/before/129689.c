bool ResourceFetcher::defersLoading() const
{
    if (LocalFrame* frame = this->frame())
        return frame->page()->defersLoading();
    return false;
}
