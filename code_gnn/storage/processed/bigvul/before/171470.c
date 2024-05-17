bool ASessionDescription::parseNTPRange(
 const char *s, float *npt1, float *npt2) {
 if (s[0] == '-') {
 return false;  
 }

 if (!strncmp("now", s, 3)) {
 return false;  
 }

 char *end;
 *npt1 = strtof(s, &end);

 if (end == s || *end != '-') {
 return false;
 }

    s = end + 1;  

 if (*s == '\0') {
 *npt2 = FLT_MAX;  
 return true;
 }

 if (!strncmp("now", s, 3)) {
 return false;  
 }

 *npt2 = strtof(s, &end);

 if (end == s || *end != '\0') {
 return false;
 }

 return *npt2 > *npt1;
}
