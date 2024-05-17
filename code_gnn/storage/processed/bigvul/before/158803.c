static bool MakesCycle(HTMLImport* parent, const KURL& url) {
  for (HTMLImport* ancestor = parent; ancestor; ancestor = ancestor->Parent()) {
    if (!ancestor->IsRoot() &&
        EqualIgnoringFragmentIdentifier(ToHTMLImportChild(parent)->Url(), url))
      return true;
  }

  return false;
}
