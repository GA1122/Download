BackgroundLoaderOffliner* BackgroundLoaderOffliner::FromWebContents(
    content::WebContents* contents) {
  Offliner* offliner = OfflinerUserData::OfflinerFromWebContents(contents);
  if (offliner)
    return static_cast<BackgroundLoaderOffliner*>(offliner);
  return nullptr;
}
