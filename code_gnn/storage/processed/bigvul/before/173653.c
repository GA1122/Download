const uint8_t *ATSParser::PSISection::data() const {
 return mBuffer == NULL ? NULL : mBuffer->data();
}
