void WaitForInterstitialDetach(content::WebContents* web_contents) {
  RunTaskAndWaitForInterstitialDetach(web_contents, base::Closure());
}
