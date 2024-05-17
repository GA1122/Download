mrb_run(mrb_state *mrb, struct RProc *proc, mrb_value self)
{
  if (mrb->c->ci->argc < 0) {
    return mrb_vm_run(mrb, proc, self, 3);  
  }
  else {
    return mrb_vm_run(mrb, proc, self, mrb->c->ci->argc + 2);  
  }
}
