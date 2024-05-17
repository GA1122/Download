bool HasInternalURL(const NavigationEntry* entry) {
  if (!entry)
    return false;

  if (entry->GetVirtualURL().SchemeIs(chrome::kChromeUIScheme))
    return true;

  if (entry->GetVirtualURL().SchemeIs(chrome::kViewSourceScheme))
    return entry->GetURL().SchemeIs(chrome::kChromeUIScheme);

  return false;
}
