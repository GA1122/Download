 bool ExtensionViewGuest::NavigateGuest(const std::string& src,
                                        bool force_navigation) {
   GURL url = extension_url_.Resolve(src);
  
  bool url_not_allowed = (url != GURL(url::kAboutBlankURL)) &&
      (url.GetOrigin() != extension_url_.GetOrigin());
//   bool url_not_allowed = url != GURL(url::kAboutBlankURL) &&
//                          !url::IsSameOriginWith(url, extension_url_);
    if (!url.is_valid() || url_not_allowed)
      return NavigateGuest(url::kAboutBlankURL, true  );
  
   if (!force_navigation && (url_ == url))
     return false;
 
   web_contents()->GetRenderProcessHost()->FilterURL(false, &url);
   web_contents()->GetController().LoadURL(url, content::Referrer(),
                                           ui::PAGE_TRANSITION_AUTO_TOPLEVEL,
                                           std::string());
 
   url_ = url;
   return true;
 }