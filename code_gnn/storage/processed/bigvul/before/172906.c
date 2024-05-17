static void hex_dump(char *msg, void *data, int size, int trunc)
{
 unsigned char *p = data;
 unsigned char c;
 int n;
 char bytestr[4] = {0};
 char addrstr[10] = {0};
 char hexstr[ 16*3 + 5] = {0};
 char charstr[16*1 + 5] = {0};

    bdt_log("%s  \n", msg);

  
 if(trunc && (size>32))
        size = 32;

 for(n=1;n<=size;n++) {
 if (n%16 == 1) {
  
            snprintf(addrstr, sizeof(addrstr), "%.4x",
 (unsigned int)((uintptr_t)p-(uintptr_t)data) );
 }

        c = *p;
 if (isalnum(c) == 0) {
            c = '.';
 }

  
        snprintf(bytestr, sizeof(bytestr), "%02X ", *p);
        strncat(hexstr, bytestr, sizeof(hexstr)-strlen(hexstr)-1);

  
        snprintf(bytestr, sizeof(bytestr), "%c", c);
        strncat(charstr, bytestr, sizeof(charstr)-strlen(charstr)-1);

 if(n%16 == 0) {
  
            bdt_log("[%4.4s]   %-50.50s  %s\n", addrstr, hexstr, charstr);
            hexstr[0] = 0;
            charstr[0] = 0;
 } else if(n%8 == 0) {
  
            strncat(hexstr, "  ", sizeof(hexstr)-strlen(hexstr)-1);
            strncat(charstr, " ", sizeof(charstr)-strlen(charstr)-1);
 }
        p++;  
 }

 if (strlen(hexstr) > 0) {
  
        bdt_log("[%4.4s]   %-50.50s  %s\n", addrstr, hexstr, charstr);
 }
}
