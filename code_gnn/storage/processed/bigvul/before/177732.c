std::string StringPrintf(std::string str, int d) {
 char buf[30];
    sprintf(buf, str.c_str(), d);
 return std::string(buf);
}
