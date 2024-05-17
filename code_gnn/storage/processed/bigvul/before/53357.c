JSON_read(int fd)
{
    uint32_t hsize, nsize;
    char *str;
    cJSON *json = NULL;

    if (Nread(fd, (char*) &nsize, sizeof(nsize), Ptcp) >= 0) {
	hsize = ntohl(nsize);
	str = (char *) malloc(hsize+1);	 
	if (str != NULL) {
	    if (Nread(fd, str, hsize, Ptcp) >= 0) {
		str[hsize] = '\0';	 
		json = cJSON_Parse(str);
	    }
	}
	free(str);
    }
    return json;
}
