  bool LoadPdf(const GURL& url) {
    ui_test_utils::NavigateToURL(browser(), url);
    content::WebContents* web_contents =
        browser()->tab_strip_model()->GetActiveWebContents();
    return pdf_extension_test_util::EnsurePDFHasLoaded(web_contents);
  }
