WKPageRef TestController::createOtherPage(WKPageRef oldPage, WKURLRequestRef, WKDictionaryRef, WKEventModifiers, WKEventMouseButton, const void* clientInfo)
{
    PlatformWebView* parentView = static_cast<PlatformWebView*>(const_cast<void*>(clientInfo));

    PlatformWebView* view = new PlatformWebView(WKPageGetContext(oldPage), WKPageGetPageGroup(oldPage), parentView->options());
    WKPageRef newPage = view->page();

    view->resizeTo(800, 600);

    WKPageUIClient otherPageUIClient = {
        kWKPageUIClientCurrentVersion,
        view,
        0,  
        0,  
        closeOtherPage,
        0,  
        focus,
        unfocus,
        0,  
        0,  
        0,  
        0,  
        0,  
        0,  
        0,  
        0,  
        0,  
        0,  
        0,  
        0,  
        0,  
        0,  
        0,  
        0,  
        getWindowFrame,
        setWindowFrame,
        runBeforeUnloadConfirmPanel,
        0,  
        0,  
        exceededDatabaseQuota,
        0,  
        decidePolicyForGeolocationPermissionRequest,
        0,  
        0,  
        0,  
        0,  
        0,  
        runModal,
        0,  
        0,  
        0,  
        createOtherPage,
        0,  
        0,  
        0,  
        0,  
        0,  
        0,  
    };
    WKPageSetPageUIClient(newPage, &otherPageUIClient);

    WKRetain(newPage);
    return newPage;
}
