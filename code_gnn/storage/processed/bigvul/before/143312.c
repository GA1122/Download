ChromeClient& Frame::chromeClient() const
{
    if (Page* page = this->page())
        return page->chromeClient();
    return emptyChromeClient();
}
