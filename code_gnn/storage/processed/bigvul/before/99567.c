std::string GetListName(int list_id) {
  if (list_id == MALWARE)
    return kMalwareList;
   return (list_id == PHISH) ? kPhishingList : std::string();
 }
