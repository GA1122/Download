void initialize_string8()
{
    gDarwinIsReallyAnnoying = gDarwinCantLoadAllObjects;

 SharedBuffer* buf = SharedBuffer::alloc(1);
 char* str = (char*)buf->data();
 *str = 0;
    gEmptyStringBuf = buf;
    gEmptyString = str;
}
