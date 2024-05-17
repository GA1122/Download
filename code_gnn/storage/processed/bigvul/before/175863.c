size_t fillNlAttrU32(__u16 nlaType, int32_t value, nlattr* nlAttr, uint32_t* u32Value) {
 *u32Value = htonl(value);
 return fillNlAttr(nlaType, sizeof((*u32Value)), nlAttr);
}
