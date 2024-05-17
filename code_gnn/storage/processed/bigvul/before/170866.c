GraphicBuffer::~GraphicBuffer()
{
 if (handle) {
        free_handle();
 }
}
