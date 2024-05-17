Vector<IconURL> Document::IconURLs(int icon_types_mask) {
  IconURL first_favicon;
  IconURL first_touch_icon;
  IconURL first_touch_precomposed_icon;
  Vector<IconURL> secondary_icons;

  using TraversalFunction = HTMLLinkElement* (*)(const Node&);
  TraversalFunction find_next_candidate =
      &Traversal<HTMLLinkElement>::NextSibling;

  HTMLLinkElement* first_element = nullptr;
  if (head()) {
    first_element = Traversal<HTMLLinkElement>::FirstChild(*head());
  } else if (IsSVGDocument() && IsSVGSVGElement(documentElement())) {
    first_element = Traversal<HTMLLinkElement>::FirstWithin(*documentElement());
    find_next_candidate = &Traversal<HTMLLinkElement>::Next;
  }

  for (HTMLLinkElement* link_element = first_element; link_element;
       link_element = find_next_candidate(*link_element)) {
    if (!(link_element->GetIconType() & icon_types_mask))
      continue;
    if (link_element->Href().IsEmpty())
      continue;

    IconURL new_url(link_element->Href(), link_element->IconSizes(),
                    link_element->GetType(), link_element->GetIconType());
    if (link_element->GetIconType() == kFavicon) {
      if (first_favicon.icon_type_ != kInvalidIcon)
        secondary_icons.push_back(first_favicon);
      first_favicon = new_url;
    } else if (link_element->GetIconType() == kTouchIcon) {
      if (first_touch_icon.icon_type_ != kInvalidIcon)
        secondary_icons.push_back(first_touch_icon);
      first_touch_icon = new_url;
    } else if (link_element->GetIconType() == kTouchPrecomposedIcon) {
      if (first_touch_precomposed_icon.icon_type_ != kInvalidIcon)
        secondary_icons.push_back(first_touch_precomposed_icon);
      first_touch_precomposed_icon = new_url;
    } else {
      NOTREACHED();
    }
  }

  Vector<IconURL> icon_urls;
  if (first_favicon.icon_type_ != kInvalidIcon)
    icon_urls.push_back(first_favicon);
  else if (url_.ProtocolIsInHTTPFamily() && icon_types_mask & kFavicon)
    icon_urls.push_back(IconURL::DefaultFavicon(url_));

  if (first_touch_icon.icon_type_ != kInvalidIcon)
    icon_urls.push_back(first_touch_icon);
  if (first_touch_precomposed_icon.icon_type_ != kInvalidIcon)
    icon_urls.push_back(first_touch_precomposed_icon);
  for (int i = secondary_icons.size() - 1; i >= 0; --i)
    icon_urls.push_back(secondary_icons[i]);
  return icon_urls;
}
