read_hexstring(const char *l2string, u_char *hex, const int hexlen)
{
    int numbytes = 0;
    unsigned int value;
    char *l2byte;
    u_char databyte;
    char *token = NULL;
    char *string;

    string = safe_strdup(l2string);

    if (hexlen <= 0)
        err(-1, "Hex buffer must be > 0");

    memset(hex, '\0', hexlen);

     

     
    l2byte = strtok_r(string, ",", &token);
    sscanf(l2byte, "%x", &value);
    if (value > 0xff)
        errx(-1, "Invalid hex string byte: %s", l2byte);
    databyte = (u_char) value;
    memcpy(&hex[numbytes], &databyte, 1);

     
    while ((l2byte = strtok_r(NULL, ",", &token)) != NULL) {
        numbytes++;
        if (numbytes + 1 > hexlen) {
            warn("Hex buffer too small for data- skipping data");
            goto done;
        }
        sscanf(l2byte, "%x", &value);
        if (value > 0xff)
            errx(-1, "Invalid hex string byte: %s", l2byte);
        databyte = (u_char) value;
        memcpy(&hex[numbytes], &databyte, 1);
    }

    numbytes++;

done:
    safe_free(string);

    dbgx(1, "Read %d bytes of hex data", numbytes);
    return (numbytes);
}