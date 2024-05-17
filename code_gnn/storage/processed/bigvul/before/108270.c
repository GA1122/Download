void FrameLoader::navigateToDifferentDocument(HistoryItem* item, FrameLoadType loadType)
{
    history()->setProvisionalItem(item);

    if (CachedPage* cachedPage = pageCache()->get(item)) {
        loadWithDocumentLoader(cachedPage->documentLoader(), loadType, 0);   
        return;
    }

    KURL itemURL = item->url();
    KURL itemOriginalURL = item->originalURL();
    KURL currentURL;
    if (documentLoader())
        currentURL = documentLoader()->url();
    RefPtr<FormData> formData = item->formData();

    bool addedExtraFields = false;
    ResourceRequest request(itemURL);

    if (!item->referrer().isNull())
        request.setHTTPReferrer(item->referrer());
    
    NavigationAction action;
    if (formData) {
        formData->generateFiles(m_frame->document());

        request.setHTTPMethod("POST");
        request.setHTTPBody(formData);
        request.setHTTPContentType(item->formContentType());
        RefPtr<SecurityOrigin> securityOrigin = SecurityOrigin::createFromString(item->referrer());
        addHTTPOriginIfNeeded(request, securityOrigin->toString());

        addExtraFieldsToRequest(request, m_loadType, true, formData);
        addedExtraFields = true;
        
        
        if (ResourceHandle::willLoadFromCache(request, m_frame))
            action = NavigationAction(itemURL, loadType, false);
        else {
            request.setCachePolicy(ReloadIgnoringCacheData);
            action = NavigationAction(itemURL, NavigationTypeFormResubmitted);
        }
    } else {
        switch (loadType) {
            case FrameLoadTypeReload:
            case FrameLoadTypeReloadFromOrigin:
                request.setCachePolicy(ReloadIgnoringCacheData);
                break;
            case FrameLoadTypeBack:
            case FrameLoadTypeBackWMLDeckNotAccessible:
            case FrameLoadTypeForward:
            case FrameLoadTypeIndexedBackForward:
                if (m_stateMachine.committedFirstRealDocumentLoad() && !itemURL.protocolIs("https"))
                    request.setCachePolicy(ReturnCacheDataElseLoad);
                break;
            case FrameLoadTypeStandard:
            case FrameLoadTypeRedirectWithLockedBackForwardList:
                break;
            case FrameLoadTypeSame:
            default:
                ASSERT_NOT_REACHED();
        }

        action = NavigationAction(itemOriginalURL, loadType, false);
    }
    
    if (!addedExtraFields)
        addExtraFieldsToRequest(request, m_loadType, true, formData);

    loadWithNavigationAction(request, action, false, loadType, 0);
}
