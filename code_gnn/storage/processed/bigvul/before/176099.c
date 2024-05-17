bool SkIcoCodec::IsIco(const void* buffer, size_t bytesRead) {
 const char icoSig[] = { '\x00', '\x00', '\x01', '\x00' };
 const char curSig[] = { '\x00', '\x00', '\x02', '\x00' };
 return bytesRead >= sizeof(icoSig) &&
 (!memcmp(buffer, icoSig, sizeof(icoSig)) ||
 !memcmp(buffer, curSig, sizeof(curSig)));
}
