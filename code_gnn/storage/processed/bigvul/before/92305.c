copyString(const XML_Char *s,
           const XML_Memory_Handling_Suite *memsuite)
{
    int charsRequired = 0;
    XML_Char *result;

     
    while (s[charsRequired] != 0) {
      charsRequired++;
    }
     
    charsRequired++;

     
    result = memsuite->malloc_fcn(charsRequired * sizeof(XML_Char));
    if (result == NULL)
        return NULL;
     
    memcpy(result, s, charsRequired * sizeof(XML_Char));
    return result;
}
