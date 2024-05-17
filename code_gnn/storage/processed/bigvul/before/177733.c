void addString(std::vector<std::string>* list, std::string str) {
 if (str.compare(0, 6, "DIRECT") == 0) {
 list->push_back("DIRECT");
 } else if (str.compare(0, 6, "PROXY ") == 0) {
 list->push_back(str.substr(6));
 } else {
    ALOGE("Unrecognized proxy string");
 }
}
