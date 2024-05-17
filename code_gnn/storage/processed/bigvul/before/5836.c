static bool ehci_verify_pid(EHCIQueue *q, EHCIqtd *qtd)
{
    int ep  = get_field(q->qh.epchar, QH_EPCHAR_EP);
    int pid = ehci_get_pid(qtd);

     
    if (q->last_pid && ep != 0 && pid != q->last_pid) {
        return false;
    } else {
        return true;
    }
}
