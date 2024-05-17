void String8::clear() {
 SharedBuffer::bufferFromData(mString)->release();
    mString = getEmptyString();
}
