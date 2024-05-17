char* skip_host_serial(const char* service) {
 static const std::vector<std::string>& prefixes =
 *(new std::vector<std::string>{"usb:", "product:", "model:", "device:"});

 for (const std::string& prefix : prefixes) {
 if (!strncmp(service, prefix.c_str(), prefix.length())) {
 return strchr(service + prefix.length(), ':');
 }
 }

 if (!strncmp(service, "tcp:", 4) || !strncmp(service, "udp:", 4)) {
        service += 4;
 }

 char* first_colon = strchr(service, ':');
 if (!first_colon) {
 return nullptr;
 }

 char* serial_end = first_colon;
 if (isdigit(serial_end[1])) {
        serial_end++;
 while (*serial_end && isdigit(*serial_end)) {
            serial_end++;
 }
 if (*serial_end != ':') {
            serial_end = first_colon;
 }
 }
 return serial_end;
}
