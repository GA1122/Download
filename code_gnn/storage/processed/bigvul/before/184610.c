 void TestController::createWebViewWithOptions(WKDictionaryRef options)
 {
     m_mainWebView = adoptPtr(new PlatformWebView(m_context.get(), m_pageGroup.get(), options));
     WKPageUIClient pageUIClient = {
         kWKPageUIClientCurrentVersion,
         m_mainWebView.get(),
         0,  
         0,  
         0,  
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
         decidePolicyForNotificationPermissionRequest,  
         unavailablePluginButtonClicked,
         0,  
         0,  
         0,  
     };
     WKPageSetPageUIClient(m_mainWebView->page(), &pageUIClient);
 
     WKPageLoaderClient pageLoaderClient = {
         kWKPageLoaderClientCurrentVersion,
         this,
         0,  
         0,  
         0,  
         didCommitLoadForFrame,
         0,  
         didFinishLoadForFrame,
         0,  
         0,  
         0,  
         0,  
         0,  
         0,  
         0,  
         0,  
         0,  
         didReceiveAuthenticationChallengeInFrame,  
         0,  
         0,  
         0,  
         0,  
         0,  
         processDidCrash,
         0,  
         0,  
         0,  
         0,  
         0,  
          0,  
          0,  
          0,  
//         0,  
//         0,  
          0,  
      };
      WKPageSetPageLoaderClient(m_mainWebView->page(), &pageLoaderClient);
 
     WKPagePolicyClient pagePolicyClient = {
         kWKPagePolicyClientCurrentVersion,
         this,
         decidePolicyForNavigationAction,
         0,  
         decidePolicyForResponse,
         0,  
     };
     WKPageSetPagePolicyClient(m_mainWebView->page(), &pagePolicyClient);
 }