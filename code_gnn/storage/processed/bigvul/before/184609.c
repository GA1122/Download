 InjectedBundlePage::InjectedBundlePage(WKBundlePageRef page)
     : m_page(page)
     , m_world(AdoptWK, WKBundleScriptWorldCreateWorld())
 {
     WKBundlePageLoaderClient loaderClient = {
         kWKBundlePageLoaderClientCurrentVersion,
         this,
         didStartProvisionalLoadForFrame,
         didReceiveServerRedirectForProvisionalLoadForFrame,
         didFailProvisionalLoadWithErrorForFrame,
         didCommitLoadForFrame,
         didFinishDocumentLoadForFrame,
         didFinishLoadForFrame,
         didFailLoadWithErrorForFrame,
         didSameDocumentNavigationForFrame,
         didReceiveTitleForFrame,
         0,  
         0,  
         0,  
         didDisplayInsecureContentForFrame,
         didRunInsecureContentForFrame,
         didClearWindowForFrame,
         didCancelClientRedirectForFrame,
         willPerformClientRedirectForFrame,
         didHandleOnloadEventsForFrame,
         0,  
         0,  
         didDetectXSSForFrame,
         0,  
         0,  
         0,  
         0,  
          0,  
          didFinishProgress,  
          0,  
//         0,  
//         0,  
          0,  
      };
      WKBundlePageSetPageLoaderClient(m_page, &loaderClient);
 
     WKBundlePageResourceLoadClient resourceLoadClient = {
         kWKBundlePageResourceLoadClientCurrentVersion,
         this,
         didInitiateLoadForResource,
         willSendRequestForFrame,
         didReceiveResponseForResource,
         didReceiveContentLengthForResource,
         didFinishLoadForResource,
         didFailLoadForResource,
         shouldCacheResponse,
         0  
     };
     WKBundlePageSetResourceLoadClient(m_page, &resourceLoadClient);
 
     WKBundlePagePolicyClient policyClient = {
         kWKBundlePagePolicyClientCurrentVersion,
         this,
         decidePolicyForNavigationAction,
         decidePolicyForNewWindowAction,
         decidePolicyForResponse,
         unableToImplementPolicy
     };
     WKBundlePageSetPolicyClient(m_page, &policyClient);
 
     WKBundlePageUIClient uiClient = {
         kWKBundlePageUIClientCurrentVersion,
         this,
         willAddMessageToConsole,
         willSetStatusbarText,
         willRunJavaScriptAlert,
         willRunJavaScriptConfirm,
         willRunJavaScriptPrompt,
         0,  
         0,  
         0,  
         0,  
         0,  
         0,  
         0,  
         0,  
         0,  
         didReachApplicationCacheOriginQuota,
         didExceedDatabaseQuota,
         0,  
         0,  
         0,  
     };
     WKBundlePageSetUIClient(m_page, &uiClient);
 
     WKBundlePageEditorClient editorClient = {
         kWKBundlePageEditorClientCurrentVersion,
         this,
         shouldBeginEditing,
         shouldEndEditing,
         shouldInsertNode,
         shouldInsertText,
         shouldDeleteRange,
         shouldChangeSelectedRange,
         shouldApplyStyle,
         didBeginEditing,
         didEndEditing,
         didChange,
         didChangeSelection,
         0,  
         0,  
         0   
     };
     WKBundlePageSetEditorClient(m_page, &editorClient);
 
 #if ENABLE(FULLSCREEN_API)
     WKBundlePageFullScreenClient fullScreenClient = {
         kWKBundlePageFullScreenClientCurrentVersion,
         this,
         supportsFullScreen,
         enterFullScreenForElement,
         exitFullScreenForElement,
         beganEnterFullScreen,
         beganExitFullScreen,
         closeFullScreen,
     };
     WKBundlePageSetFullScreenClient(m_page, &fullScreenClient);
 #endif
 }