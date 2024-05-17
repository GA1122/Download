bool ATSParser::PSISection::isEmpty() const {
 return mBuffer == NULL || mBuffer->size() == 0;
}
