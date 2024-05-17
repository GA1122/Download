bool AreURLsInPageNavigation(const GURL& existing_url,
                             const GURL& new_url,
                             bool renderer_says_in_page,
                             NavigationType navigation_type) {
  if (existing_url == new_url)
    return renderer_says_in_page;

  if (!new_url.has_ref()) {
    return navigation_type == NAVIGATION_TYPE_IN_PAGE;
  }

  url_canon::Replacements<char> replacements;
  replacements.ClearRef();
  return existing_url.ReplaceComponents(replacements) ==
      new_url.ReplaceComponents(replacements);
}
