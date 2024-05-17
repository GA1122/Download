size_t ATSParser::PSISection::size() const {
 return mBuffer == NULL ? 0 : mBuffer->size();
}
