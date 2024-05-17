size_t fillNlAttr(__u16 nlaType, size_t valueSize, nlattr* nlAttr) {
 size_t dataLen = valueSize;
 int padLength = NLMSG_ALIGN(dataLen) - dataLen;
    nlAttr->nla_len = (__u16)(dataLen + sizeof(nlattr));
    nlAttr->nla_type = nlaType;
 return padLength;
}
