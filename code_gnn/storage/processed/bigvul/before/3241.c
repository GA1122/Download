s_aes_set_key(stream_aes_state * state, const unsigned char *key,
                  int keylength)
{
    int code = 0;

    if ( (keylength < 1) || (keylength > SAES_MAX_KEYLENGTH) )
        return_error(gs_error_rangecheck);
    if (key == NULL)
        return_error(gs_error_invalidaccess);

     
    memcpy(state->key, key, keylength);
    state->keylength = keylength;

    if (code) {
      return gs_throw(gs_error_rangecheck, "could not set AES key");
    }

     
    return 0;
}
