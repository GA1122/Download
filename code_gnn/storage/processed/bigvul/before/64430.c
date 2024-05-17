mark_context(mrb_state *mrb, struct mrb_context *c)
{
  int i;
  mrb_callinfo *ci;

   
  mark_context_stack(mrb, c);

   
  if (c->cibase) {
    for (ci = c->cibase; ci <= c->ci; ci++) {
      mrb_gc_mark(mrb, (struct RBasic*)ci->env);
      mrb_gc_mark(mrb, (struct RBasic*)ci->proc);
      mrb_gc_mark(mrb, (struct RBasic*)ci->target_class);
    }
  }
   
  for (i=0; i<c->esize; i++) {
    if (c->ensure[i] == NULL) break;
    mrb_gc_mark(mrb, (struct RBasic*)c->ensure[i]);
  }
   
  if (c->prev && c->prev->fib) {
    mrb_gc_mark(mrb, (struct RBasic*)c->prev->fib);
  }
}
