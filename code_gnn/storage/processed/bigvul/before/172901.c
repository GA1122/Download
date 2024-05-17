void get_bdaddr(const char *str, bt_bdaddr_t *bd) {
 char *d = ((char *)bd), *endp;
 int i;
 for(i = 0; i < 6; i++) {
 *d++ = strtol(str, &endp, 16);
 if (*endp != ':' && i != 5) {
            memset(bd, 0, sizeof(bt_bdaddr_t));
 return;
 }
        str = endp + 1;
 }
}
