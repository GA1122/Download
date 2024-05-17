string16 DatabaseUtil::GetOriginIdentifier(const GURL& url) {
  string16 spec = UTF8ToUTF16(url.spec());
  return WebKit::WebSecurityOrigin::createFromString(spec).databaseIdentifier();
}
