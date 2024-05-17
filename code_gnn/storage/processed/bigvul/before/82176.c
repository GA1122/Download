mrb_f_block_given_p_m(mrb_state *mrb, mrb_value self)
{
  mrb_callinfo *ci = &mrb->c->ci[-1];
  mrb_callinfo *cibase = mrb->c->cibase;
  mrb_value *bp;
  struct RProc *p;

  if (ci <= cibase) {
     
    return mrb_false_value();
  }
  p = ci->proc;
   
  while (p) {
    if (MRB_PROC_SCOPE_P(p)) break;
    p = p->upper;
  }
  if (p == NULL) return mrb_false_value();
   
  while (cibase < ci) {
    if (ci->proc == p) break;
    ci--;
  }
  if (ci == cibase) {
    return mrb_false_value();
  }
  else if (ci->env) {
    struct REnv *e = ci->env;
    int bidx;

     
    if (e->stack == mrb->c->stbase)
      return mrb_false_value();
     
    bidx = MRB_ENV_BIDX(e);
     
    if (bidx >= MRB_ENV_STACK_LEN(e))
      return mrb_false_value();
    bp = &e->stack[bidx];
  }
  else {
    bp = ci[1].stackent+1;
    if (ci->argc >= 0) {
      bp += ci->argc;
    }
    else {
      bp++;
    }
  }
  if (mrb_nil_p(*bp))
    return mrb_false_value();
  return mrb_true_value();
}
