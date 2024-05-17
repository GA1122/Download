    StartTagScanner(const StringImpl* tagImpl, MediaValuesCached* mediaValues)
        : m_tagImpl(tagImpl)
        , m_linkIsStyleSheet(false)
        , m_linkTypeIsMissingOrSupportedStyleSheet(true)
        , m_linkIsPreconnect(false)
        , m_linkIsPreload(false)
        , m_linkIsImport(false)
        , m_matchedMediaAttribute(true)
        , m_inputIsImage(false)
        , m_sourceSize(0)
        , m_sourceSizeSet(false)
        , m_defer(FetchRequest::NoDefer)
        , m_crossOrigin(CrossOriginAttributeNotSet)
        , m_mediaValues(mediaValues)
        , m_referrerPolicySet(false)
        , m_referrerPolicy(ReferrerPolicyDefault)
    {
        if (match(m_tagImpl, imgTag)
            || match(m_tagImpl, sourceTag)) {
            m_sourceSize = SizesAttributeParser(m_mediaValues, String()).length();
            return;
        }
        if ( !match(m_tagImpl, inputTag)
            && !match(m_tagImpl, linkTag)
            && !match(m_tagImpl, scriptTag)
            && !match(m_tagImpl, videoTag))
            m_tagImpl = 0;
    }
