resp_parse(netdissect_options *ndo, register const u_char *bp, int length)
{
    u_char op;
    int ret_len;

    LCHECK2(length, 1);
    ND_TCHECK(*bp);
    op = *bp;

     
    switch(op) {
        case RESP_SIMPLE_STRING:  ret_len = resp_print_simple_string(ndo, bp, length);   break;
        case RESP_INTEGER:        ret_len = resp_print_integer(ndo, bp, length);         break;
        case RESP_ERROR:          ret_len = resp_print_error(ndo, bp, length);           break;
        case RESP_BULK_STRING:    ret_len = resp_print_bulk_string(ndo, bp, length);     break;
        case RESP_ARRAY:          ret_len = resp_print_bulk_array(ndo, bp, length);      break;
        default:                  ret_len = resp_print_inline(ndo, bp, length);          break;
    }

     
    TEST_RET_LEN(ret_len);

trunc:
    return (-1);
}
