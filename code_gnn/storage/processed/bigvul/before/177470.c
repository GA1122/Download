long long ReadID(IMkvReader* pReader, long long pos, long& len) {
 const long long id = ReadUInt(pReader, pos, len);
 if (id < 0 || len < 1 || len > 4) {
 return E_FILE_FORMAT_INVALID;
 }
 return id;
}
