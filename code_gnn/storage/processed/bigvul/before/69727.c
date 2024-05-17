getinfo_helper_format_single_entry_guard(const entry_guard_t *e)
{
  const char *status = NULL;
  time_t when = 0;
  const node_t *node;
  char tbuf[ISO_TIME_LEN+1];
  char nbuf[MAX_VERBOSE_NICKNAME_LEN+1];

   
  if (e->confirmed_idx < 0) {
    status = "never-connected";
  } else if (! e->currently_listed) {
    when = e->unlisted_since_date;
    status = "unusable";
  } else if (! e->is_filtered_guard) {
    status = "unusable";
  } else if (e->is_reachable == GUARD_REACHABLE_NO) {
    when = e->failing_since;
    status = "down";
  } else {
    status = "up";
  }

  node = entry_guard_find_node(e);
  if (node) {
    node_get_verbose_nickname(node, nbuf);
  } else {
    nbuf[0] = '$';
    base16_encode(nbuf+1, sizeof(nbuf)-1, e->identity, DIGEST_LEN);
     
  }

  char *result = NULL;
  if (when) {
    format_iso_time(tbuf, when);
    tor_asprintf(&result, "%s %s %s\n", nbuf, status, tbuf);
  } else {
    tor_asprintf(&result, "%s %s\n", nbuf, status);
  }
  return result;
}
