bool xmp_serialize(XmpPtr xmp, XmpStringPtr buffer, uint32_t options,
                   uint32_t padding)
{
    RESET_ERROR;
    return xmp_serialize_and_format(xmp, buffer, options, padding, "\n", " ",
                                    0);
}
