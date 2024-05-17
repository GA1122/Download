 void GLES2Util::MapUint64ToTwoUint32(
     uint64_t v64, uint32_t* v32_0, uint32_t* v32_1) {
  DCHECK(v32_0 && v32_1);
  *v32_0 = static_cast<uint32_t>(v64 & 0xFFFFFFFF);
  *v32_1 = static_cast<uint32_t>((v64 & 0xFFFFFFFF00000000) >> 32);
}
