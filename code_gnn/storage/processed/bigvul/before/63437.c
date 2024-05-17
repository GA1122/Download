finish_preauth(void *arg, krb5_error_code code)
{
    struct as_req_state *state = arg;
    krb5_error_code real_code = code;

    if (code) {
        if (vague_errors)
            code = KRB5KRB_ERR_GENERIC;
        state->status = "PREAUTH_FAILED";
        if (real_code == KRB5KDC_ERR_PREAUTH_FAILED) {
            state->preauth_err = code;
            get_preauth_hint_list(state->request, &state->rock, &state->e_data,
                                  finish_missing_required_preauth, state);
            return;
        }
    } else {
         
        state->status = missing_required_preauth(state->client, state->server,
                                                 &state->enc_tkt_reply);
        if (state->status) {
            state->preauth_err = KRB5KDC_ERR_PREAUTH_REQUIRED;
            get_preauth_hint_list(state->request, &state->rock, &state->e_data,
                                  finish_missing_required_preauth, state);
            return;
        }
    }

    finish_process_as_req(state, code);
}