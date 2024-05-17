void String8::setTo(const String8& other)
{
 SharedBuffer::bufferFromData(other.mString)->acquire();
 SharedBuffer::bufferFromData(mString)->release();
    mString = other.mString;
}
