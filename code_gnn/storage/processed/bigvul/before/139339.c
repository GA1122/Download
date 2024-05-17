bool AddressListFromListValue(const base::ListValue* value, AddressList* list) {
  list->clear();
  for (base::ListValue::const_iterator it = value->begin(); it != value->end();
       it++) {
    IPAddress address;
    std::string addr_string;
    if (!it->GetAsString(&addr_string) ||
        !address.AssignFromIPLiteral(addr_string)) {
      return false;
    }
    list->push_back(IPEndPoint(address, 0));
  }
  return true;
}
