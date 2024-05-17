    PassOwnPtr<PreloadRequest> createPreloadRequest(const KURL& predictedBaseURL, const SegmentedString& source, const ClientHintsPreferences& clientHintsPreferences, const PictureData& pictureData, const ReferrerPolicy documentReferrerPolicy)
    {
        PreloadRequest::RequestType requestType = PreloadRequest::RequestTypePreload;
        if (shouldPreconnect())
            requestType = PreloadRequest::RequestTypePreconnect;
        else if (isLinkRelPreload())
            requestType = PreloadRequest::RequestTypeLinkRelPreload;
        else if (!shouldPreload() || !m_matchedMediaAttribute)
            return nullptr;

        TextPosition position = TextPosition(source.currentLine(), source.currentColumn());
        FetchRequest::ResourceWidth resourceWidth;
        float sourceSize = m_sourceSize;
        bool sourceSizeSet = m_sourceSizeSet;
        if (pictureData.picked) {
            sourceSizeSet = pictureData.sourceSizeSet;
            sourceSize = pictureData.sourceSize;
        }
        if (sourceSizeSet) {
            resourceWidth.width = sourceSize;
            resourceWidth.isSet = true;
        }

        Resource::Type type;
        if (!resourceType(type))
            return nullptr;

        ReferrerPolicy referrerPolicy = (m_referrerPolicy != ReferrerPolicyDefault) ? m_referrerPolicy : documentReferrerPolicy;
        OwnPtr<PreloadRequest> request = PreloadRequest::create(initiatorFor(m_tagImpl), position, m_urlToLoad, predictedBaseURL, type, referrerPolicy, resourceWidth, clientHintsPreferences, requestType);
        request->setCrossOrigin(m_crossOrigin);
        request->setCharset(charset());
        request->setDefer(m_defer);
        request->setIntegrityMetadata(m_integrityMetadata);
        return request.release();
    }
