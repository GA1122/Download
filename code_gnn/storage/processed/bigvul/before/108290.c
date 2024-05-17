void FrameLoader::urlSelected(const KURL& url, const String& passedTarget, PassRefPtr<Event> triggeringEvent, bool lockHistory, bool lockBackForwardList, bool userGesture, ReferrerPolicy referrerPolicy)
{
    urlSelected(ResourceRequest(url), passedTarget, triggeringEvent, lockHistory, lockBackForwardList, userGesture, referrerPolicy, DoNotReplaceDocumentIfJavaScriptURL);
}
