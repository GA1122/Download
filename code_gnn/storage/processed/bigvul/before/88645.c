static void dwc3_ep_inc_deq(struct dwc3_ep *dep)
{
 	dwc3_ep_inc_trb(&dep->trb_dequeue);
 }