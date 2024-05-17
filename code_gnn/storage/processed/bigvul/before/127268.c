void PageSerializer::addFontToResources(FontResource* font)
{
    if (!font || !shouldAddURL(font->url()) || !font->isLoaded() || !font->resourceBuffer()) {
        return;
    }
    RefPtr<SharedBuffer> data(font->resourceBuffer());

    addToResources(font, data, font->url());
}
