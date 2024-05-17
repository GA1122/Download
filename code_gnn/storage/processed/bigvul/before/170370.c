size_t MPEG4Source::parseNALSize(const uint8_t *data) const {
 switch (mNALLengthSize) {
 case 1:
 return *data;
 case 2:
 return U16_AT(data);
 case 3:
 return ((size_t)data[0] << 16) | U16_AT(&data[1]);
 case 4:
 return U32_AT(data);
 }

    CHECK(!"Should not be here.");

 return 0;
}
