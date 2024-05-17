bool HostCache::RestoreFromListValue(const base::ListValue& old_cache) {
  for (auto it = old_cache.begin(); it != old_cache.end(); it++) {
    const base::DictionaryValue* entry_dict;
    if (!it->GetAsDictionary(&entry_dict))
      return false;

    std::string hostname;
    int address_family;
    HostResolverFlags flags;
    int error = OK;
    std::string expiration;
    base::ListValue empty_list;
    const base::ListValue* addresses_value = &empty_list;
    AddressList address_list;

    if (!entry_dict->GetString(kHostnameKey, &hostname) ||
        !entry_dict->GetInteger(kFlagsKey, &flags) ||
        !entry_dict->GetInteger(kAddressFamilyKey, &address_family) ||
        !entry_dict->GetString(kExpirationKey, &expiration)) {
      return false;
    }

    if (!entry_dict->GetInteger(kErrorKey, &error) &&
        !entry_dict->GetList(kAddressesKey, &addresses_value)) {
      return false;
    }

    int64_t time_internal;
    if (!base::StringToInt64(expiration, &time_internal))
      return false;

    base::TimeTicks expiration_time =
        base::TimeTicks::Now() -
        (base::Time::Now() - base::Time::FromInternalValue(time_internal));

    Key key(hostname, static_cast<AddressFamily>(address_family), flags);
    if (error == OK &&
        !AddressListFromListValue(addresses_value, &address_list)) {
      return false;
    }

    auto found = entries_.find(key);
    if (found == entries_.end() && size() < max_entries_) {
      AddEntry(key, Entry(error, address_list, expiration_time,
                          network_changes_ - 1));
    }
  }
  return true;
}
