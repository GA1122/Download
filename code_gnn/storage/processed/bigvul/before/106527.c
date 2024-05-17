WebCore::IntRect WebPageProxy::firstRectForCharacterInSelectedRange(int characterPosition)
{
    IntRect resultRect;
    process()->sendSync(Messages::WebPage::FirstRectForCharacterInSelectedRange(characterPosition), Messages::WebPage::FirstRectForCharacterInSelectedRange::Reply(resultRect), m_pageID);
    return resultRect;
}
