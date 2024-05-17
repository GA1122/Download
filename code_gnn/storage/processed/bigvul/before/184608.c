 QtBuiltinBundlePage::QtBuiltinBundlePage(QtBuiltinBundle* bundle, WKBundlePageRef page)
     : m_bundle(bundle)
     , m_page(page)
     , m_navigatorQtObject(0)
     , m_navigatorQtObjectEnabled(false)
 {
     WKBundlePageLoaderClient loaderClient = {
         kWKBundlePageLoaderClientCurrentVersion,
         this,
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
         didClearWindowForFrame,
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
//         0,  
//         0,  
          0,  
      };
      WKBundlePageSetPageLoaderClient(m_page, &loaderClient);
 }