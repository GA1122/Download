tokenize(apr_pool_t *pool, const char *str, bool ignore_whitespace,
             char **error)
{
    apr_array_header_t *tokens = apr_array_make(pool, 10, sizeof(Token));
    const char *p, *start;

    *error = NULL;
    p = start = str;
    while(*p) {
        if (apr_isspace(*p)) {   
            p++;
            while(*p && apr_isspace(*p)) p++;
            if (!ignore_whitespace) {
                push_token(tokens, TOKEN_WHITESPACE, str, start, p);
            }
            start = p;
        }
        else if (apr_isalpha(*p)) {  
            p++;
            while(*p && (apr_isalnum(*p) || *p == '_')) p++;
            push_token(tokens, TOKEN_IDENTIFIER, str, start, p);
            start = p;
        }
        else if (*p == '"') {    
            p++;                 
            while(*p) {
                if (*p == '\\') {  
                    p++;           
                    if (*p) {
                        p++;       
                    } else {
                        break;     
                    }
                }
                if (*p == '\"') break;  
                p++;                    
            }
            if (*p != '\"') {
                *error = apr_psprintf(pool,
                                      "unterminated string beginning at "
                                      "position %" APR_SIZE_T_FMT " in \"%s\"",
                                      start-str, str);
                break;
            }
            p++;
            push_token(tokens, TOKEN_DBL_QUOTE_STRING, str, start, p);
            start = p;
        }
        else if (*p == '=') {    
            p++;
            push_token(tokens, TOKEN_EQUAL, str, start, p);
            start = p;
        }
        else if (*p == ',') {    
            p++;
            push_token(tokens, TOKEN_COMMA, str, start, p);
            start = p;
        }
        else if (*p == ';') {    
            p++;
            push_token(tokens, TOKEN_SEMICOLON, str, start, p);
            start = p;
        }
        else {                   
            *error = apr_psprintf(pool,
                                  "unknown token at "
                                  "position %" APR_SIZE_T_FMT " in string \"%s\"",
                                  p-str, str);
            break;
        }
    }

    return tokens;
}
