static int find_str_in_buffer(
	char const *buffer,
	int buffer_len,
	char const *str)
{
	int str_len = strlen(str);
    int i;
	for(i=0; i+str_len <= buffer_len; i++) {
		if( memcmp(buffer+i,str,str_len)==0 ) {
			return i;
		}
	}
	return -1;
}
