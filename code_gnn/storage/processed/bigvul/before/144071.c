png_set_asm_flags (png_structp png_ptr, png_uint_32 asm_flags)
{
 
    if (png_ptr != NULL)
    png_ptr->asm_flags = 0;
    PNG_UNUSED(asm_flags)  
}
