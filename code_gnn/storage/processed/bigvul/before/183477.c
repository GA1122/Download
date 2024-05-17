 bool MatchFilter::MatchesDomain(const std::string& domain) {
   if (!details_->HasKey(keys::kDomainKey))
     return true;
 
   std::string filter_value;
   if (!details_->GetString(keys::kDomainKey, &filter_value))
      return false;
    if (net::CookieMonster::DomainIsHostOnly(filter_value))
    filter_value = "." + filter_value;
//     filter_value.insert(0, ".");
  
    std::string sub_domain(domain);
    if (!net::CookieMonster::DomainIsHostOnly(sub_domain))
      sub_domain = sub_domain.substr(1);
  
  for (sub_domain = "." + sub_domain;
       sub_domain.length() >= filter_value.length();
       ) {
//    
//   for (sub_domain.insert(0, ".");
//        sub_domain.length() >= filter_value.length();) {
      if (sub_domain == filter_value)
        return true;
      const size_t next_dot = sub_domain.find('.', 1);   
     sub_domain.erase(0, next_dot);
   }
   return false;
 }