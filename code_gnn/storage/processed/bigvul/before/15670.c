static int IRQ_get_next(OpenPICState *opp, IRQQueue *q)
{
     
    IRQ_check(opp, q);

    return q->next;
}
