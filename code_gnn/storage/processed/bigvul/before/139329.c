void HarfBuzzShaper::setFontFeatures()
{
    const FontDescription& description = m_font->fontDescription();

    static hb_feature_t noKern = { HB_TAG('k', 'e', 'r', 'n'), 0, 0, static_cast<unsigned>(-1) };
    static hb_feature_t noVkrn = { HB_TAG('v', 'k', 'r', 'n'), 0, 0, static_cast<unsigned>(-1) };
    switch (description.kerning()) {
    case FontDescription::NormalKerning:
        break;
    case FontDescription::NoneKerning:
        m_features.append(description.isVerticalAnyUpright() ? noVkrn : noKern);
        break;
    case FontDescription::AutoKerning:
        break;
    }

    static hb_feature_t noClig = { HB_TAG('c', 'l', 'i', 'g'), 0, 0, static_cast<unsigned>(-1) };
    static hb_feature_t noLiga = { HB_TAG('l', 'i', 'g', 'a'), 0, 0, static_cast<unsigned>(-1) };
    switch (description.commonLigaturesState()) {
    case FontDescription::DisabledLigaturesState:
        m_features.append(noLiga);
        m_features.append(noClig);
        break;
    case FontDescription::EnabledLigaturesState:
        break;
    case FontDescription::NormalLigaturesState:
        break;
    }
    static hb_feature_t dlig = { HB_TAG('d', 'l', 'i', 'g'), 1, 0, static_cast<unsigned>(-1) };
    switch (description.discretionaryLigaturesState()) {
    case FontDescription::DisabledLigaturesState:
        break;
    case FontDescription::EnabledLigaturesState:
        m_features.append(dlig);
        break;
    case FontDescription::NormalLigaturesState:
        break;
    }
    static hb_feature_t hlig = { HB_TAG('h', 'l', 'i', 'g'), 1, 0, static_cast<unsigned>(-1) };
    switch (description.historicalLigaturesState()) {
    case FontDescription::DisabledLigaturesState:
        break;
    case FontDescription::EnabledLigaturesState:
        m_features.append(hlig);
        break;
    case FontDescription::NormalLigaturesState:
        break;
    }
    static hb_feature_t noCalt = { HB_TAG('c', 'a', 'l', 't'), 0, 0, static_cast<unsigned>(-1) };
    switch (description.contextualLigaturesState()) {
    case FontDescription::DisabledLigaturesState:
        m_features.append(noCalt);
        break;
    case FontDescription::EnabledLigaturesState:
        break;
    case FontDescription::NormalLigaturesState:
        break;
    }

    static hb_feature_t hwid = { HB_TAG('h', 'w', 'i', 'd'), 1, 0, static_cast<unsigned>(-1) };
    static hb_feature_t twid = { HB_TAG('t', 'w', 'i', 'd'), 1, 0, static_cast<unsigned>(-1) };
    static hb_feature_t qwid = { HB_TAG('q', 'w', 'i', 'd'), 1, 0, static_cast<unsigned>(-1) };
    switch (description.widthVariant()) {
    case HalfWidth:
        m_features.append(hwid);
        break;
    case ThirdWidth:
        m_features.append(twid);
        break;
    case QuarterWidth:
        m_features.append(qwid);
        break;
    case RegularWidth:
        break;
    }

    FontFeatureSettings* settings = description.featureSettings();
    if (!settings)
        return;

    unsigned numFeatures = settings->size();
    for (unsigned i = 0; i < numFeatures; ++i) {
        hb_feature_t feature;
        const AtomicString& tag = settings->at(i).tag();
        feature.tag = HB_TAG(tag[0], tag[1], tag[2], tag[3]);
        feature.value = settings->at(i).value();
        feature.start = 0;
        feature.end = static_cast<unsigned>(-1);
        m_features.append(feature);
    }
}
