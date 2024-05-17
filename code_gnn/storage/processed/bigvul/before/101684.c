bool HasInternalURL(const NavigationEntry* entry) {
  if (!entry)
    return false;

  if (entry->virtual_url().SchemeIs(chrome::kChromeUIScheme))
    return true;

  if (entry->virtual_url().SchemeIs(chrome::kViewSourceScheme))
    return entry->url().SchemeIs(chrome::kChromeUIScheme);

  return false;
}
