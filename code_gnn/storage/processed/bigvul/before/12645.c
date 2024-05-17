eval_acl(uschar ** sub, int nsub, uschar ** user_msgp)
{
int i;
uschar *tmp;
int sav_narg = acl_narg;
int ret;
extern int acl_where;

if(--nsub > sizeof(acl_arg)/sizeof(*acl_arg)) nsub = sizeof(acl_arg)/sizeof(*acl_arg);
for (i = 0; i < nsub && sub[i+1]; i++)
  {
  tmp = acl_arg[i];
  acl_arg[i] = sub[i+1];	 
  sub[i+1] = tmp;		 
  }
acl_narg = i;
while (i < nsub)
  {
  sub[i+1] = acl_arg[i];
  acl_arg[i++] = NULL;
  }

DEBUG(D_expand)
  debug_printf("expanding: acl: %s  arg: %s%s\n",
    sub[0],
    acl_narg>0 ? acl_arg[0] : US"<none>",
    acl_narg>1 ? " +more"   : "");

ret = acl_eval(acl_where, sub[0], user_msgp, &tmp);

for (i = 0; i < nsub; i++)
  acl_arg[i] = sub[i+1];	 
acl_narg = sav_narg;

return ret;
}
