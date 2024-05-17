char* String8::lockBuffer(size_t size)
{
 SharedBuffer* buf = SharedBuffer::bufferFromData(mString)
 ->editResize(size+1);
 if (buf) {
 char* str = (char*)buf->data();
        mString = str;
 return str;
 }
 return NULL;
}
