rend_service_parse_intro_for_v3(
    rend_intro_cell_t *intro,
    const uint8_t *buf,
    size_t plaintext_len,
    char **err_msg_out)
{
  ssize_t adjust, v2_ver_specific_len, ts_offset;

   
  if (intro->version != 3) {
    if (err_msg_out)
      tor_asprintf(err_msg_out,
                   "rend_service_parse_intro_for_v3() called with "
                   "bad version %d on INTRODUCE%d cell (this is a bug)",
                   intro->version,
                   (int)(intro->type));
    goto err;
  }

   
  if (plaintext_len < 4) {
    if (err_msg_out) {
      tor_asprintf(err_msg_out,
                   "truncated plaintext of encrypted parted of "
                   "version %d INTRODUCE%d cell",
                   intro->version,
                   (int)(intro->type));
    }

    goto err;
  }

   

  intro->u.v3.auth_type = buf[1];
  if (intro->u.v3.auth_type != REND_NO_AUTH) {
    intro->u.v3.auth_len = ntohs(get_uint16(buf + 2));
    ts_offset = 4 + intro->u.v3.auth_len;
  } else {
    intro->u.v3.auth_len = 0;
    ts_offset = 2;
  }

   
  if (intro->u.v3.auth_type == REND_BASIC_AUTH ||
      intro->u.v3.auth_type == REND_STEALTH_AUTH) {
      if (intro->u.v3.auth_len != REND_DESC_COOKIE_LEN) {
        if (err_msg_out) {
          tor_asprintf(err_msg_out,
                       "wrong auth data size %d for INTRODUCE%d cell, "
                       "should be %d",
                       (int)(intro->u.v3.auth_len),
                       (int)(intro->type),
                       REND_DESC_COOKIE_LEN);
        }

        goto err;
      }
  }

   
  if (plaintext_len < (size_t)(ts_offset)+4) {
    if (err_msg_out) {
      tor_asprintf(err_msg_out,
                   "truncated plaintext of encrypted parted of "
                   "version %d INTRODUCE%d cell",
                   intro->version,
                   (int)(intro->type));
    }

    goto err;
  }

  if (intro->u.v3.auth_type != REND_NO_AUTH &&
      intro->u.v3.auth_len > 0) {
     
    intro->u.v3.auth_data = tor_malloc(intro->u.v3.auth_len);
     
    memcpy(intro->u.v3.auth_data, buf + 4, intro->u.v3.auth_len);
  }

   
  adjust = 3 + ts_offset;

  v2_ver_specific_len =
    rend_service_parse_intro_for_v2(intro,
                                    buf + adjust, plaintext_len - adjust,
                                    err_msg_out);

   
  if (v2_ver_specific_len >= 0) return v2_ver_specific_len + adjust;
   
  else return v2_ver_specific_len;

 err:
  return -1;
}
