static asocket* create_host_service_socket(const char* name, const char* serial) {
    asocket* s;

    s = host_service_to_socket(name, serial);

 if (s != NULL) {
        D("LS(%d) bound to '%s'", s->id, name);
 return s;
 }

 return s;
}
