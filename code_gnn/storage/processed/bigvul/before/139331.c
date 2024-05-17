bool HarfBuzzShaper::shapeHarfBuzzRuns()
{
    HarfBuzzScopedPtr<hb_buffer_t> harfBuzzBuffer(hb_buffer_create(), hb_buffer_destroy);

    HarfBuzzRunCache& runCache = harfBuzzRunCache();
    const FontDescription& fontDescription = m_font->fontDescription();
    const String& localeString = fontDescription.locale();
    CString locale = localeString.latin1();
    HarfBuzzRun* previousRun = nullptr;

    for (unsigned i = 0; i < m_harfBuzzRuns.size(); ++i) {
        unsigned runIndex = m_run.rtl() ? m_harfBuzzRuns.size() - i - 1 : i;
        HarfBuzzRun* currentRun = m_harfBuzzRuns[runIndex].get();

        const SimpleFontData* currentFontData = currentRun->fontData();
        FontPlatformData* platformData = const_cast<FontPlatformData*>(&currentFontData->platformData());
        HarfBuzzFace* face = platformData->harfBuzzFace();
        if (!face)
            return false;

        hb_buffer_set_language(harfBuzzBuffer.get(), hb_language_from_string(locale.data(), locale.length()));
        hb_buffer_set_script(harfBuzzBuffer.get(), currentRun->script());
        hb_buffer_set_direction(harfBuzzBuffer.get(), currentRun->direction());

        const UChar* src = m_normalizedBuffer.get() + currentRun->startIndex();
        std::wstring key(src, src + currentRun->numCharacters());

        CachedShapingResults* cachedResults = runCache.find(key);
        if (cachedResults) {
            if (isValidCachedResult(m_font, currentRun->direction(),
                localeString, cachedResults)) {
                currentRun->applyShapeResult(cachedResults->buffer);
                setGlyphPositionsForHarfBuzzRun(currentRun,
                    cachedResults->buffer, previousRun);
                hb_buffer_clear_contents(harfBuzzBuffer.get());
                runCache.moveToBack(cachedResults);
                previousRun = currentRun;
                continue;
            }
            runCache.remove(cachedResults);
        }

        static const uint16_t preContext = space;
        hb_buffer_add_utf16(harfBuzzBuffer.get(), &preContext, 1, 1, 0);

        addToHarfBuzzBufferInternal(harfBuzzBuffer.get(),
            fontDescription, m_normalizedBuffer.get(), currentRun->startIndex(),
            currentRun->numCharacters());

        if (fontDescription.isVerticalAnyUpright())
            face->setScriptForVerticalGlyphSubstitution(harfBuzzBuffer.get());

        HarfBuzzScopedPtr<hb_font_t> harfBuzzFont(face->createFont(), hb_font_destroy);

        hb_shape(harfBuzzFont.get(), harfBuzzBuffer.get(), m_features.isEmpty() ? 0 : m_features.data(), m_features.size());
        currentRun->applyShapeResult(harfBuzzBuffer.get());
        setGlyphPositionsForHarfBuzzRun(currentRun, harfBuzzBuffer.get(), previousRun);

        runCache.insert(key, new CachedShapingResults(harfBuzzBuffer.get(), m_font, currentRun->direction(), localeString));

        harfBuzzBuffer.set(hb_buffer_create());

        previousRun = currentRun;
    }


    return true;
}
