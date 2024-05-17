static int ehci_state_advqueue(EHCIQueue *q)
{
#if 0
     
    if (I-bit set) {
        ehci_set_state(ehci, async, EST_HORIZONTALQH);
        goto out;
    }
#endif

     
    if (((q->qh.token & QTD_TOKEN_TBYTES_MASK) != 0) &&
        (NLPTR_TBIT(q->qh.altnext_qtd) == 0)) {
        q->qtdaddr = q->qh.altnext_qtd;
        ehci_set_state(q->ehci, q->async, EST_FETCHQTD);

     
    } else if (NLPTR_TBIT(q->qh.next_qtd) == 0) {
        q->qtdaddr = q->qh.next_qtd;
        ehci_set_state(q->ehci, q->async, EST_FETCHQTD);

     
    } else {
        ehci_set_state(q->ehci, q->async, EST_HORIZONTALQH);
    }

    return 1;
}
