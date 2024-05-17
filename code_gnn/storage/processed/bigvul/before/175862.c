size_t fillNlAttrIpAddress(__u16 nlaType, int family, const std::string& value, nlattr* nlAttr,
 Slice ipAddress) {
    inet_pton(family, value.c_str(), ipAddress.base());
 return fillNlAttr(nlaType, (family == AF_INET) ? sizeof(in_addr) : sizeof(in6_addr), nlAttr);
}
