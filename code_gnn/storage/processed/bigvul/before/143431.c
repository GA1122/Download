    void handlePictureSourceURL(PictureData& pictureData)
    {
        if (match(m_tagImpl, sourceTag) && m_matchedMediaAttribute && pictureData.sourceURL.isEmpty()) {
            pictureData.sourceURL = m_srcsetImageCandidate.toString();
            pictureData.sourceSizeSet = m_sourceSizeSet;
            pictureData.sourceSize = m_sourceSize;
            pictureData.picked = true;
        } else if (match(m_tagImpl, imgTag) && !pictureData.sourceURL.isEmpty()) {
            setUrlToLoad(pictureData.sourceURL, AllowURLReplacement);
        }
    }
