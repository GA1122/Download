rend_service_decrypt_intro(
    rend_intro_cell_t *intro,
    crypto_pk_t *key,
    char **err_msg_out)
{
  char *err_msg = NULL;
  uint8_t key_digest[DIGEST_LEN];
  char service_id[REND_SERVICE_ID_LEN_BASE32+1];
  ssize_t key_len;
  uint8_t buf[RELAY_PAYLOAD_SIZE];
  int result, status = -1;

  if (!intro || !key) {
    if (err_msg_out) {
      err_msg =
        tor_strdup("rend_service_decrypt_intro() called with bad "
                   "parameters");
    }

    status = -2;
    goto err;
  }

   
  if (!(intro->ciphertext) || intro->ciphertext_len <= 0) {
    if (err_msg_out) {
      tor_asprintf(&err_msg,
                   "rend_intro_cell_t was missing ciphertext for "
                   "INTRODUCE%d cell",
                   (int)(intro->type));
    }
    status = -3;
    goto err;
  }

   

   
  crypto_pk_get_digest(key, (char *)key_digest);
  if (tor_memneq(key_digest, intro->pk, DIGEST_LEN)) {
    if (err_msg_out) {
      base32_encode(service_id, REND_SERVICE_ID_LEN_BASE32 + 1,
                    (char*)(intro->pk), REND_SERVICE_ID_LEN);
      tor_asprintf(&err_msg,
                   "got an INTRODUCE%d cell for the wrong service (%s)",
                   (int)(intro->type),
                   escaped(service_id));
    }

    status = -4;
    goto err;
  }

   

  key_len = crypto_pk_keysize(key);
  if (intro->ciphertext_len < key_len) {
    if (err_msg_out) {
      tor_asprintf(&err_msg,
                   "got an INTRODUCE%d cell with a truncated PK-encrypted "
                   "part",
                   (int)(intro->type));
    }

    status = -5;
    goto err;
  }

   

  note_crypto_pk_op(REND_SERVER);
  result =
    crypto_pk_private_hybrid_decrypt(
       key, (char *)buf, sizeof(buf),
       (const char *)(intro->ciphertext), intro->ciphertext_len,
       PK_PKCS1_OAEP_PADDING, 1);
  if (result < 0) {
    if (err_msg_out) {
      tor_asprintf(&err_msg,
                   "couldn't decrypt INTRODUCE%d cell",
                   (int)(intro->type));
    }
    status = -6;
    goto err;
  }
  intro->plaintext_len = result;
  intro->plaintext = tor_malloc(intro->plaintext_len);
  memcpy(intro->plaintext, buf, intro->plaintext_len);

  status = 0;

  goto done;

 err:
  if (err_msg_out && !err_msg) {
    tor_asprintf(&err_msg,
                 "unknown INTRODUCE%d error decrypting encrypted part",
                 intro ? (int)(intro->type) : -1);
  }

 done:
  if (err_msg_out) *err_msg_out = err_msg;
  else tor_free(err_msg);

   
  memwipe(buf, 0, sizeof(buf));
  memwipe(key_digest, 0, sizeof(key_digest));
  memwipe(service_id, 0, sizeof(service_id));

  return status;
}
