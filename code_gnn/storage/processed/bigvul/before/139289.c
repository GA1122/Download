CachedShapingResults::CachedShapingResults(hb_buffer_t* harfBuzzBuffer, const Font* fontData, hb_direction_t dirData, const String& newLocale)
    : buffer(harfBuzzBuffer)
    , font(*fontData)
    , dir(dirData)
    , locale(newLocale)
{
}
