void FrameLoader::didBeginDocument()
{
    ASSERT(m_frame);
    ASSERT(m_frame->document());
    ASSERT(m_frame->document()->fetcher());

    if (m_documentLoader) {
        String suboriginHeader = m_documentLoader->response().httpHeaderField(HTTPNames::Suborigin);
        if (!suboriginHeader.isNull()) {
            Vector<String> messages;
            Suborigin suborigin;
            if (parseSuboriginHeader(suboriginHeader, &suborigin, messages))
                m_frame->document()->enforceSuborigin(suborigin);

            for (auto& message : messages)
                m_frame->document()->addConsoleMessage(ConsoleMessage::create(SecurityMessageSource, ErrorMessageLevel, "Error with Suborigin header: " + message));
        }
        m_frame->document()->clientHintsPreferences().updateFrom(m_documentLoader->clientHintsPreferences());
    }

    Settings* settings = m_frame->document()->settings();
    if (settings) {
        m_frame->document()->fetcher()->setImagesEnabled(settings->imagesEnabled());
        m_frame->document()->fetcher()->setAutoLoadImages(settings->loadsImagesAutomatically());
    }

    if (m_documentLoader) {
        const AtomicString& dnsPrefetchControl = m_documentLoader->response().httpHeaderField(HTTPNames::X_DNS_Prefetch_Control);
        if (!dnsPrefetchControl.isEmpty())
            m_frame->document()->parseDNSPrefetchControlHeader(dnsPrefetchControl);

        String headerContentLanguage = m_documentLoader->response().httpHeaderField(HTTPNames::Content_Language);
        if (!headerContentLanguage.isEmpty()) {
            size_t commaIndex = headerContentLanguage.find(',');
            headerContentLanguage.truncate(commaIndex);  
            headerContentLanguage = headerContentLanguage.stripWhiteSpace(isHTMLSpace<UChar>);
            if (!headerContentLanguage.isEmpty())
                m_frame->document()->setContentLanguage(AtomicString(headerContentLanguage));
        }

        OriginTrialContext::addTokensFromHeader(m_frame->document(), m_documentLoader->response().httpHeaderField(HTTPNames::Origin_Trial));
    }

    client()->didCreateNewDocument();
}
