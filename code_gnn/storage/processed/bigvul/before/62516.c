resp_print_bulk_string(netdissect_options *ndo, register const u_char *bp, int length) {
    int length_cur = length, string_len;

     
    SKIP_OPCODE(bp, length_cur);

     
    GET_LENGTH(ndo, length_cur, bp, string_len);

    if (string_len >= 0) {
         
        if (string_len == 0)
            resp_print_empty(ndo);
        else {
            LCHECK2(length_cur, string_len);
            ND_TCHECK2(*bp, string_len);
            RESP_PRINT_SEGMENT(ndo, bp, string_len);
            bp += string_len;
            length_cur -= string_len;
        }

         
        FIND_CRLF(bp, length_cur);
        CONSUME_CRLF(bp, length_cur);
    } else {
         
        switch(string_len) {
            case (-1):  resp_print_null(ndo);             break;
            case (-2):  goto trunc;
            case (-3):  resp_print_length_too_large(ndo); break;
            case (-4):  resp_print_length_negative(ndo);  break;
            default:    resp_print_invalid(ndo);          break;
        }
    }

    return (length - length_cur);

trunc:
    return (-1);
}
