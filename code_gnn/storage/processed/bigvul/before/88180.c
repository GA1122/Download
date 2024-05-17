XML_ExpatVersion(void) {
   
   

#define V1(a, b, c) XML_L(#a) XML_L(".") XML_L(#b) XML_L(".") XML_L(#c)
#define V2(a, b, c) XML_L("expat_") V1(a, b, c)

  return V2(XML_MAJOR_VERSION, XML_MINOR_VERSION, XML_MICRO_VERSION);

#undef V1
#undef V2
}
