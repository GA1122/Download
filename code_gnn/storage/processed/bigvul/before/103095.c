TabContentsWrapper* Browser::CreateTabContentsForURL(
    const GURL& url, const GURL& referrer, Profile* profile,
    PageTransition::Type transition, bool defer_load,
    SiteInstance* instance) const {
  TabContentsWrapper* contents = TabContentsFactory(profile, instance,
      MSG_ROUTING_NONE,
      GetSelectedTabContents(), NULL);
  if (!defer_load) {
    contents->controller().LoadURL(url, referrer, transition);
  }

  return contents;
}
