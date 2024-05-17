    bool resourceType(Resource::Type& type) const
    {
        if (match(m_tagImpl, scriptTag)) {
            type = Resource::Script;
        } else if (match(m_tagImpl, imgTag) || match(m_tagImpl, videoTag) || (match(m_tagImpl, inputTag) && m_inputIsImage)) {
            type = Resource::Image;
        } else if (match(m_tagImpl, linkTag) && m_linkIsStyleSheet) {
            type = Resource::CSSStyleSheet;
        } else if (m_linkIsPreconnect) {
            type = Resource::Raw;
        } else if (m_linkIsPreload) {
            if (!LinkLoader::getResourceTypeFromAsAttribute(m_asAttributeValue, type))
                return false;
        } else if (match(m_tagImpl, linkTag) && m_linkIsImport) {
            type = Resource::ImportResource;
        } else {
            ASSERT_NOT_REACHED();
        }
        return true;
    }
