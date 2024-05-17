e1000e_eitr_should_postpone(E1000ECore *core, int idx)
{
    return e1000e_postpone_interrupt(&core->eitr_intr_pending[idx],
                                     &core->eitr[idx]);
}
