init_user_id_from_FQN (const char * _fqn) {

  char * uid = NULL;
  char * domain = NULL;
  char * fqn = NULL;
  char default_uid [] = "nobody";

  if (_fqn) {
    fqn = strdup (_fqn);
    uid = fqn;

    char * pAt = strchr (fqn, '@');
    if (pAt) {
      *pAt='\0';
      domain = pAt+1;
    }
  }
  
  if (uid == NULL) {
    uid = default_uid;
  }

  int rc = init_user_ids (uid, domain);
  dprintf (D_FULLDEBUG, "Switching to user %s@%s, result = %d\n", uid, domain, rc);

  if (fqn)
    free (fqn);

  return rc;
}
