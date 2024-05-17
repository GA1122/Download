resp_print(netdissect_options *ndo, const u_char *bp, u_int length)
{
    int ret_len = 0, length_cur = length;

    if(!bp || length <= 0)
        return;

    ND_PRINT((ndo, ": RESP"));
    while (length_cur > 0) {
         
        ret_len = resp_parse(ndo, bp, length_cur);
        TEST_RET_LEN_NORETURN(ret_len);
        bp += ret_len;
        length_cur -= ret_len;
    }

    return;

trunc:
    ND_PRINT((ndo, "%s", tstr));
}
