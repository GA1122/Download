bool Document::IsInWebAppScope() const {
  if (!GetSettings())
    return false;

  const String& web_app_scope = GetSettings()->GetWebAppScope();
  if (web_app_scope.IsNull() || web_app_scope.IsEmpty())
    return false;

  DCHECK_EQ(KURL(web_app_scope).GetString(), web_app_scope);
  return Url().GetString().StartsWith(web_app_scope);
}
