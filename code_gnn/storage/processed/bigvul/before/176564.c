xmlCreateFileParserCtxt(const char *filename)
{
 return(xmlCreateURLParserCtxt(filename, 0));
}