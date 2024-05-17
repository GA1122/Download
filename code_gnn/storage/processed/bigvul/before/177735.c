std::vector<std::string> string16ToProxyList(String16 response) {
    std::vector<std::string> ret;
 String8 response8(response);
    std::string rstr(response8.string());
 if (rstr.find(';') == std::string::npos) {
        addString(&ret, rstr);
 return ret;
 }
 char str[128];
    rstr.copy(str, 0, rstr.length());
 const char* pch = strtok(str, ";");

 while (pch != NULL) {
 while ((*pch) == ' ') ++pch;
        std::string pstring(pch);
        addString(&ret, pstring);

        pch = strtok(NULL, "; \t");
 }

 return ret;
}
