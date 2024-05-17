resp_print_string_error_integer(netdissect_options *ndo, register const u_char *bp, int length) {
    int length_cur = length, len, ret_len;
    const u_char *bp_ptr;

     
    SKIP_OPCODE(bp, length_cur);
    bp_ptr = bp;

     
    FIND_CRLF(bp_ptr, length_cur);

     
    len = (bp_ptr - bp);
    RESP_PRINT_SEGMENT(ndo, bp, len);
    ret_len = 1   + len   + 2  ;

    TEST_RET_LEN(ret_len);

trunc:
    return (-1);
}
