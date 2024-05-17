void CSSCallbackWebFrameClient::didMatchCSS(WebFrame* frame, const WebVector<WebString>& newlyMatchingSelectors, const WebVector<WebString>& stoppedMatchingSelectors)
{
    ++m_updateCount;
    std::set<std::string>& frameSelectors = m_matchedSelectors[frame];
    for (size_t i = 0; i < newlyMatchingSelectors.size(); ++i) {
        std::string selector = newlyMatchingSelectors[i].utf8();
        EXPECT_EQ(0U, frameSelectors.count(selector)) << selector;
        frameSelectors.insert(selector);
    }
    for (size_t i = 0; i < stoppedMatchingSelectors.size(); ++i) {
        std::string selector = stoppedMatchingSelectors[i].utf8();
        EXPECT_EQ(1U, frameSelectors.count(selector)) << selector;
        frameSelectors.erase(selector);
    }
}
