Profile* ProfileForWebContents(content::WebContents* web_contents) {
  if (!web_contents)
    return NULL;
  return Profile::FromBrowserContext(web_contents->GetBrowserContext());
}
