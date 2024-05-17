static bool IsSoftwareCodec(const char *componentName) {
 if (!strncmp("OMX.google.", componentName, 11)) {
 return true;
 }

 if (!strncmp("OMX.", componentName, 4)) {
 return false;
 }

 return true;
}
