status_t String8::unlockBuffer(size_t size)
{
 if (size != this->size()) {
 SharedBuffer* buf = SharedBuffer::bufferFromData(mString)
 ->editResize(size+1);
 if (! buf) {
 return NO_MEMORY;
 }

 char* str = (char*)buf->data();
        str[size] = 0;
        mString = str;
 }

 return NO_ERROR;
}
