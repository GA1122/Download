ReadFile(Buffer *buffer, FILE *input)
{
	     char	buf[BUFSIZ + 1];
    register int	bytes;

    buffer->used = 0;
    while (!feof(input) && (bytes = fread(buf, 1, BUFSIZ, input)) > 0) {
#ifdef WIN32
	char *p;
	buf[bytes] = '\0';
	for (p = buf; p = strchr(p, '\r'); ) {
	    if (p[-1] == '\\' && p[1] == '\n') {
		bytes -= 3;
		strcpy(p - 1, p + 2);
	    }
	}
#endif
	AppendToBuffer(buffer, buf, bytes);
    }
    AppendToBuffer(buffer, "", 1);
}
