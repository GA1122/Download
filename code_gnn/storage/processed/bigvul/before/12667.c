modify_variable(uschar *name, void * value)
{
var_entry * vp;
if ((vp = find_var_ent(name))) vp->value = value;
return;           
}
