void PacketBypassCallback(Packet *p)
{
     
    int state = SC_ATOMIC_GET(p->flow->flow_state);
    if ((state == FLOW_STATE_LOCAL_BYPASSED) ||
           (state == FLOW_STATE_CAPTURE_BYPASSED)) {
        return;
    }

    if (p->BypassPacketsFlow && p->BypassPacketsFlow(p)) {
        FlowUpdateState(p->flow, FLOW_STATE_CAPTURE_BYPASSED);
    } else {
        FlowUpdateState(p->flow, FLOW_STATE_LOCAL_BYPASSED);
    }
}
