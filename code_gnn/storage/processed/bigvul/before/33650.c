pkinit_alg_agility_kdf(krb5_context context,
                       krb5_data *secret,
                       krb5_data *alg_oid,
                       krb5_const_principal party_u_info,
                       krb5_const_principal party_v_info,
                       krb5_enctype enctype,
                       krb5_data *as_req,
                       krb5_data *pk_as_rep,
                       krb5_keyblock *key_block)
{
    krb5_error_code retval = 0;

    unsigned int reps = 0;
    uint32_t counter = 1;        
    size_t offset = 0;
    size_t hash_len = 0;
    size_t rand_len = 0;
    size_t key_len = 0;
    krb5_data random_data;
    krb5_sp80056a_other_info other_info_fields;
    krb5_pkinit_supp_pub_info supp_pub_info_fields;
    krb5_data *other_info = NULL;
    krb5_data *supp_pub_info = NULL;
    krb5_algorithm_identifier alg_id;
    const EVP_MD *(*EVP_func)(void);

     
    random_data.length = 0;
    random_data.data = NULL;

     
    key_block->magic = 0;
    key_block->enctype = enctype;
    if (0 != (retval = krb5_c_keylengths(context, enctype, &rand_len,
                                         &key_len)))
        goto cleanup;

    random_data.length = rand_len;
    key_block->length = key_len;

    if (NULL == (key_block->contents = malloc(key_block->length))) {
        retval = ENOMEM;
        goto cleanup;
    }

    memset (key_block->contents, 0, key_block->length);

     
    if (party_u_info && krb5_principal_compare_any_realm(context, party_u_info,
                                                         krb5_anonymous_principal()))
        party_u_info = (krb5_principal)krb5_anonymous_principal();

    if (0 != (retval = pkinit_alg_values(context, alg_oid, &hash_len, &EVP_func)))
        goto cleanup;

     
    reps = key_block->length/hash_len;

     
    if (key_block->length > (reps * hash_len))
        reps++;

     
    if (NULL == (random_data.data = malloc(reps * hash_len))) {
        retval = ENOMEM;
        goto cleanup;
    }

     
    supp_pub_info_fields.enctype = enctype;
    supp_pub_info_fields.as_req = *as_req;
    supp_pub_info_fields.pk_as_rep = *pk_as_rep;
    if (0 != ((retval = encode_krb5_pkinit_supp_pub_info(&supp_pub_info_fields,
                                                         &supp_pub_info))))
        goto cleanup;

     
    memset(&alg_id, 0, sizeof alg_id);
    alg_id.algorithm = *alg_oid;  

    other_info_fields.algorithm_identifier = alg_id;
    other_info_fields.party_u_info = (krb5_principal) party_u_info;
    other_info_fields.party_v_info = (krb5_principal) party_v_info;
    other_info_fields.supp_pub_info = *supp_pub_info;
    if (0 != (retval = encode_krb5_sp80056a_other_info(&other_info_fields, &other_info)))
        goto cleanup;

     
    for (counter = 1; counter <= reps; counter++) {
        EVP_MD_CTX c;
        uint s = 0;
        uint32_t be_counter = htonl(counter);

        EVP_MD_CTX_init(&c);

         
        if (0 == EVP_DigestInit(&c, EVP_func())) {
            krb5_set_error_message(context, KRB5_CRYPTO_INTERNAL,
                                   "Call to OpenSSL EVP_DigestInit() returned an error.");
            retval = KRB5_CRYPTO_INTERNAL;
            goto cleanup;
        }

        if ((0 == EVP_DigestUpdate(&c, &be_counter, 4)) ||
            (0 == EVP_DigestUpdate(&c, secret->data, secret->length)) ||
            (0 == EVP_DigestUpdate(&c, other_info->data, other_info->length))) {
            krb5_set_error_message(context, KRB5_CRYPTO_INTERNAL,
                                   "Call to OpenSSL EVP_DigestUpdate() returned an error.");
            retval = KRB5_CRYPTO_INTERNAL;
            goto cleanup;
        }

         
        if (0 == EVP_DigestFinal(&c, (unsigned char *)(random_data.data + offset), &s)) {
            krb5_set_error_message(context, KRB5_CRYPTO_INTERNAL,
                                   "Call to OpenSSL EVP_DigestUpdate() returned an error.");
            retval = KRB5_CRYPTO_INTERNAL;
            goto cleanup;
        }
        offset += s;
        assert(s == hash_len);

        EVP_MD_CTX_cleanup(&c);
    }

    retval = krb5_c_random_to_key(context, enctype, &random_data,
                                  key_block);

cleanup:
     
    if (retval) {
        krb5_free_keyblock_contents(context, key_block);
    }

     
    if (random_data.data)
        free(random_data.data);
    krb5_free_data(context, other_info);
    krb5_free_data(context, supp_pub_info);

    return retval;
}  