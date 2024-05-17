 std::unique_ptr<base::ListValue> HostCache::GetAsListValue(
    bool include_staleness) const {
  std::unique_ptr<base::ListValue> entry_list(new base::ListValue());

  for (const auto& pair : entries_) {
    const Key& key = pair.first;
    const Entry& entry = pair.second;

    std::unique_ptr<base::DictionaryValue> entry_dict(
        new base::DictionaryValue());

    entry_dict->SetString(kHostnameKey, key.hostname);
    entry_dict->SetInteger(kAddressFamilyKey,
                           static_cast<int>(key.address_family));
    entry_dict->SetInteger(kFlagsKey, key.host_resolver_flags);

    if (include_staleness) {
      entry_dict->SetString(kExpirationKey,
                            NetLog::TickCountToString(entry.expires()));
      entry_dict->SetInteger(kTtlKey, entry.ttl().InMilliseconds());
      entry_dict->SetInteger(kNetworkChangesKey, entry.network_changes());
    } else {
      base::Time expiration_time =
          base::Time::Now() - (base::TimeTicks::Now() - entry.expires());
      entry_dict->SetString(
          kExpirationKey,
          base::Int64ToString(expiration_time.ToInternalValue()));
    }

    if (entry.error() != OK) {
      entry_dict->SetInteger(kErrorKey, entry.error());
    } else {
      const AddressList& addresses = entry.addresses();
      auto addresses_value = base::MakeUnique<base::ListValue>();
      for (size_t i = 0; i < addresses.size(); ++i)
        addresses_value->AppendString(addresses[i].ToStringWithoutPort());
      entry_dict->SetList(kAddressesKey, std::move(addresses_value));
    }

    entry_list->Append(std::move(entry_dict));
  }

  return entry_list;
}
