chrome::mojom::PrerenderCanceler* GetPrerenderCanceller(
    const base::Callback<content::WebContents*()>& wc_getter) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  auto* web_contents = wc_getter.Run();
  if (!web_contents)
    return nullptr;

  return prerender::PrerenderContents::FromWebContents(web_contents);
}
