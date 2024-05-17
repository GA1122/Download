void Document::addIconURL(const String& url, const String&, const String&, IconType iconType)
{
    if (url.isEmpty())
        return;

    Frame* f = frame();
    if (!f)
        return;

    f->loader()->didChangeIcons(iconType);
}
