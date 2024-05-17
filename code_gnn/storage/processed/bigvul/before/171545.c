String8::String8(StaticLinkage)
 : mString(0)
{

 char* data = static_cast<char*>(
 SharedBuffer::alloc(sizeof(char))->data());
    data[0] = 0;
    mString = data;
}
