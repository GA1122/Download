int am_urldecode(char *data)
{
    char *ip;
    char *op;
    int c1, c2;

    if (data == NULL) {
        return HTTP_BAD_REQUEST;
    }

    ip = data;
    op = data;
    while (*ip) {
        switch (*ip) {
        case '+':
            *op = ' ';
            ip++;
            op++;
            break;
        case '%':
             
            c1 = am_unhex_digit(ip[1]);
            if (c1 < 0) {
                return HTTP_BAD_REQUEST;
            }
            c2 = am_unhex_digit(ip[2]);
            if (c2 < 0) {
                return HTTP_BAD_REQUEST;
            }

            *op = (c1 << 4) | c2;
            if (*op == '\0') {
                 
                return HTTP_BAD_REQUEST;
            }
            ip += 3;
            op++;
            break;
        default:
            *op = *ip;
            ip++;
            op++;
        }
    }
    *op = '\0';

    return OK;
}
