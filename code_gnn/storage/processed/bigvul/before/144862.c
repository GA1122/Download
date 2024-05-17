std::string SerializeOriginIntoDatabaseKey(const url::Origin& origin) {
   return base::MD5String(origin.host());
 }
