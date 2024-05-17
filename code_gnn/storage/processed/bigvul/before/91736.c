push_token(apr_array_header_t *tokens, TokenType type, const char *str,
           const char *start, const char *end)
{
    apr_size_t offset = start - str;
    Token *token = apr_array_push(tokens);

    if (type == TOKEN_DBL_QUOTE_STRING) {
         
        start++; end--;
    }

    token->type = type;
    token->len = end - start;
    token->offset = offset;
    token->str = apr_pstrmemdup(tokens->pool, start, token->len);

    if (type == TOKEN_DBL_QUOTE_STRING) {
         
        char *p, *t;

        for (p = token->str; *p; p++) {
            if (p[0] == '\\' && p[1]) {
                 
                for (t = p; *t; t++) {
                    t[0] = t[1];
                }
                token->len--;
            }
        }
    }
}
