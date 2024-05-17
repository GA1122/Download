auth_action_ok(char *a,
               gid_t gid,
               gid_t *gl,
               int gn,
               gid_t egid)
{
   struct passwd *pw;
   struct group *gp;
   char *usr = NULL, **grp, *g;
   int ret, i, j;

   pw = getpwuid(uid);
   if (!pw) return 0;
   usr = pw->pw_name;
   if (!usr) return 0;
   grp = alloca(sizeof(char *) * (gn + 1 + 1));
   j = 0;
   gp = getgrgid(gid);
   if (gp)
     {
        grp[j] = gp->gr_name;
        j++;
     }
   for (i = 0; i < gn; i++)
     {
        if (gl[i] != egid)
          {
             gp = getgrgid(gl[i]);
             if (gp)
               {
                  g = alloca(strlen(gp->gr_name) + 1);
                  strcpy(g, gp->gr_name);
                  grp[j] = g;
                  j++;
               }
          }
     }
   grp[j] = NULL;
    
   ret = auth_etc_enlightenment_sysactions(a, usr, grp);
   if (ret == 1) return 1;
   else if (ret == -1)
     return 0;
    
   return 1;
}
