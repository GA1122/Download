GURL DatabaseUtil::GetOriginFromIdentifier(const string16& origin_identifier) {
  WebKit::WebSecurityOrigin web_security_origin =
      WebKit::WebSecurityOrigin::createFromDatabaseIdentifier(
          origin_identifier);

  if (web_security_origin.isUnique()) {
    if (origin_identifier.find(UTF8ToUTF16("file__")) == 0)
      return GURL("file:///");
    return GURL();
  }

   return GURL(web_security_origin.toString());
 }
