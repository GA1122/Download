e1000e_itr_should_postpone(E1000ECore *core)
{
    return e1000e_postpone_interrupt(&core->itr_intr_pending, &core->itr);
}
