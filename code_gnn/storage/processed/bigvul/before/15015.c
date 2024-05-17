ProcSetFontPath(ClientPtr client)
{
    unsigned char *ptr;
    unsigned long nbytes, total;
    long nfonts;
    int n;

    REQUEST(xSetFontPathReq);

    REQUEST_AT_LEAST_SIZE(xSetFontPathReq);

    nbytes = (client->req_len << 2) - sizeof(xSetFontPathReq);
    total = nbytes;
    ptr = (unsigned char *) &stuff[1];
    nfonts = stuff->nFonts;
    while (--nfonts >= 0) {
        if ((total == 0) || (total < (n = (*ptr + 1))))
            return BadLength;
        total -= n;
        ptr += n;
    }
    if (total >= 4)
        return BadLength;
    return SetFontPath(client, stuff->nFonts, (unsigned char *) &stuff[1]);
}
