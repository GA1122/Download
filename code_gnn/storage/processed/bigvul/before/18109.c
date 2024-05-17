auth_etc_enlightenment_sysactions(char *a,
                                  char *u,
                                  char **g)
{
   FILE *f;
   char file[4096], buf[4096], id[4096], ugname[4096], perm[4096], act[4096];
   char *p, *pp, *s, **gp;
   int ok = 0;
   size_t len, line = 0;
   int allow = 0;
   int deny = 0;

   snprintf(file, sizeof(file), "/etc/enlightenment/sysactions.conf");
   f = fopen(file, "r");
   if (!f)
     {
        snprintf(file, sizeof(file), PACKAGE_SYSCONF_DIR "/enlightenment/sysactions.conf");
        f = fopen(file, "r");
        if (!f) return 0;
     }

   auth_etc_enlightenment_sysactions_perm(file);

   while (fgets(buf, sizeof(buf), f))
     {
        line++;
        len = strlen(buf);
        if (len < 1) continue;
        if (buf[len - 1] == '\n') buf[len - 1] = 0;
         
        if (buf[0] == '#') continue;
        p = buf;
        p = get_word(p, id);
        p = get_word(p, ugname);
        pp = p;
        p = get_word(p, perm);
        allow = 0;
        deny = 0;
        if (!strcmp(id, "user:"))
          {
             if (!fnmatch(ugname, u, 0))
               {
                  if (!strcmp(perm, "allow:")) allow = 1;
                  else if (!strcmp(perm, "deny:"))
                    deny = 1;
                  else
                    goto malformed;
               }
             else
               continue;
          }
        else if (!strcmp(id, "group:"))
          {
             Eina_Bool matched = EINA_FALSE;

             for (gp = g; *gp; gp++)
               {
                  if (!fnmatch(ugname, *gp, 0))
                    {
                       matched = EINA_TRUE;
                       if (!strcmp(perm, "allow:")) allow = 1;
                       else if (!strcmp(perm, "deny:"))
                         deny = 1;
                       else
                         goto malformed;
                    }
               }
             if (!matched) continue;
          }
        else if (!strcmp(id, "action:"))
          {
             while ((*pp) && (isspace(*pp)))
               pp++;
             s = eina_hash_find(actions, ugname);
             if (s) eina_hash_del(actions, ugname, s);
             if (!actions) actions = eina_hash_string_superfast_new(free);
             eina_hash_add(actions, ugname, strdup(pp));
             continue;
          }
        else if (id[0] == 0)
          continue;
        else
          goto malformed;

        for (;; )
          {
             p = get_word(p, act);
             if (act[0] == 0) break;
             if (!fnmatch(act, a, 0))
               {
                  if (allow) ok = 1;
                  else if (deny)
                    ok = -1;
                  goto done;
               }
          }

        continue;
malformed:
        printf("WARNING: %s:%zu\n"
               "LINE: '%s'\n"
               "MALFORMED LINE. SKIPPED.\n",
               file, line, buf);
     }
done:
   fclose(f);
   return ok;
}
