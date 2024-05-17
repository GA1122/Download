Profile* ChooseProfileFromStoreId(const std::string& store_id,
                                  Profile* profile,
                                  bool include_incognito) {
  DCHECK(profile);
  if (store_id == kOriginalProfileStoreId)
    return profile->GetOriginalProfile();
  if (store_id == kOffTheRecordProfileStoreId && include_incognito)
    return profile->GetOffTheRecordProfile();
   return NULL;
 }
