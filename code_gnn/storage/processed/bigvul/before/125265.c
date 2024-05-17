static GURL ToAndFromOriginIdentifier(const GURL origin_url) {
  string16 id = DatabaseUtil::GetOriginIdentifier(origin_url);
   return DatabaseUtil::GetOriginFromIdentifier(id);
 }
