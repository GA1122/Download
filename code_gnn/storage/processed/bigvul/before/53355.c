static void lex_unget_unsave(lex_t *lex, int c)
{
    if(c != STREAM_STATE_EOF && c != STREAM_STATE_ERROR) {
         
        #ifndef NDEBUG
        char d;
        #endif
        stream_unget(&lex->stream, c);
        #ifndef NDEBUG
        d =
        #endif
            strbuffer_pop(&lex->saved_text);
        assert(c == d);
    }
}
