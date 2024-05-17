xmlURIUnescapeString(const char *str, int len, char *target) {
 char *ret, *out;
 const char *in;

 if (str == NULL)
 return(NULL);
 if (len <= 0) len = strlen(str);
 if (len < 0) return(NULL);

 if (target == NULL) {
	ret = (char *) xmlMallocAtomic(len + 1);
 if (ret == NULL) {
            xmlURIErrMemory("unescaping URI value\n");
 return(NULL);
 }
 } else
	ret = target;
    in = str;
    out = ret;
 while(len > 0) {
 if ((len > 2) && (*in == '%') && (is_hex(in[1])) && (is_hex(in[2]))) {
	    in++;
 if ((*in >= '0') && (*in <= '9'))
 *out = (*in - '0');
 else if ((*in >= 'a') && (*in <= 'f'))
 *out = (*in - 'a') + 10;
 else if ((*in >= 'A') && (*in <= 'F'))
 *out = (*in - 'A') + 10;
	    in++;
 if ((*in >= '0') && (*in <= '9'))
 *out = *out * 16 + (*in - '0');
 else if ((*in >= 'a') && (*in <= 'f'))
 *out = *out * 16 + (*in - 'a') + 10;
 else if ((*in >= 'A') && (*in <= 'F'))
 *out = *out * 16 + (*in - 'A') + 10;
	    in++;
	    len -= 3;
	    out++;
 } else {
 *out++ = *in++;
	    len--;
 }
 }
 *out = 0;
 return(ret);
}
