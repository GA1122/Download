void TokenPreloadScanner::scanCommon(const Token& token, const SegmentedString& source, PreloadRequestStream& requests, ViewportDescriptionWrapper* viewport, bool* likelyDocumentWriteScript)
{
    if (!m_documentParameters->doHtmlPreloadScanning)
        return;

    if (m_isAppCacheEnabled)
        return;

    if (m_isCSPEnabled)
        return;

    switch (token.type()) {
    case HTMLToken::Character: {
        if (m_inStyle) {
            m_cssScanner.scan(token.data(), source, requests, m_predictedBaseElementURL);
        } else if (m_inScript && likelyDocumentWriteScript && !m_didRewind) {
            *likelyDocumentWriteScript = shouldEvaluateForDocumentWrite(token.data());
        }
        return;
    }
    case HTMLToken::EndTag: {
        const StringImpl* tagImpl = tagImplFor(token.data());
        if (match(tagImpl, templateTag)) {
            if (m_templateCount)
                --m_templateCount;
            return;
        }
        if (match(tagImpl, styleTag)) {
            if (m_inStyle)
                m_cssScanner.reset();
            m_inStyle = false;
            return;
        }
        if (match(tagImpl, scriptTag)) {
            m_inScript = false;
            return;
        }
        if (match(tagImpl, pictureTag))
            m_inPicture = false;
        return;
    }
    case HTMLToken::StartTag: {
        if (m_templateCount)
            return;
        const StringImpl* tagImpl = tagImplFor(token.data());
        if (match(tagImpl, templateTag)) {
            ++m_templateCount;
            return;
        }
        if (match(tagImpl, styleTag)) {
            m_inStyle = true;
            return;
        }
        if (match(tagImpl, scriptTag)) {
            m_inScript = true;
        }
        if (match(tagImpl, baseTag)) {
            if (!m_predictedBaseElementURL.isEmpty())
                return;
            updatePredictedBaseURL(token);
            return;
        }
        if (match(tagImpl, htmlTag) && token.getAttributeItem(manifestAttr)) {
            m_isAppCacheEnabled = true;
            return;
        }
        if (match(tagImpl, metaTag)) {
            const typename Token::Attribute* equivAttribute = token.getAttributeItem(http_equivAttr);
            if (equivAttribute) {
                String equivAttributeValue(equivAttribute->value());
                if (equalIgnoringCase(equivAttributeValue, "content-security-policy")) {
                    m_isCSPEnabled = true;
                } else if (equalIgnoringCase(equivAttributeValue, "accept-ch")) {
                    const typename Token::Attribute* contentAttribute = token.getAttributeItem(contentAttr);
                    if (contentAttribute)
                        m_clientHintsPreferences.updateFromAcceptClientHintsHeader(contentAttribute->value(), nullptr);
                }
                return;
            }

            handleMetaNameAttribute(token, m_documentParameters.get(), m_mediaValues.get(), &m_cssScanner, viewport);
        }

        if (match(tagImpl, pictureTag)) {
            m_inPicture = true;
            m_pictureData = PictureData();
            return;
        }

        StartTagScanner scanner(tagImpl, m_mediaValues);
        scanner.processAttributes(token.attributes());
        if (m_inPicture)
            scanner.handlePictureSourceURL(m_pictureData);
        OwnPtr<PreloadRequest> request = scanner.createPreloadRequest(m_predictedBaseElementURL, source, m_clientHintsPreferences, m_pictureData, m_documentParameters->referrerPolicy);
        if (request)
            requests.append(request.release());
        return;
    }
    default: {
        return;
    }
    }
}
