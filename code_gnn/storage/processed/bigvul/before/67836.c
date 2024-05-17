Defrag4Reassemble(ThreadVars *tv, DefragTracker *tracker, Packet *p)
{
    Packet *rp = NULL;

     
    if (!tracker->seen_last)
        return NULL;

     
    Frag *frag;
    int len = 0;
    TAILQ_FOREACH(frag, &tracker->frags, next) {
        if (frag->skip)
            continue;

        if (frag == TAILQ_FIRST(&tracker->frags)) {
            if (frag->offset != 0) {
                goto done;
            }
            len = frag->data_len;
        }
        else {
            if (frag->offset > len) {
                 
                goto done;
            }
            else {
                len += frag->data_len;
            }
        }
    }

     
    rp = PacketDefragPktSetup(p, NULL, 0, IPV4_GET_IPPROTO(p));
    if (rp == NULL) {
        SCLogError(SC_ERR_MEM_ALLOC, "Failed to allocate packet for "
                   "fragmentation re-assembly, dumping fragments.");
        goto error_remove_tracker;
    }
    PKT_SET_SRC(rp, PKT_SRC_DEFRAG);
    rp->flags |= PKT_REBUILT_FRAGMENT;
    rp->recursion_level = p->recursion_level;

    int fragmentable_offset = 0;
    int fragmentable_len = 0;
    int hlen = 0;
    int ip_hdr_offset = 0;
    TAILQ_FOREACH(frag, &tracker->frags, next) {
        SCLogDebug("frag %p, data_len %u, offset %u, pcap_cnt %"PRIu64,
                frag, frag->data_len, frag->offset, frag->pcap_cnt);

        if (frag->skip)
            continue;
        if (frag->data_len - frag->ltrim <= 0)
            continue;
        if (frag->offset == 0) {

            if (PacketCopyData(rp, frag->pkt, frag->len) == -1)
                goto error_remove_tracker;

            hlen = frag->hlen;
            ip_hdr_offset = frag->ip_hdr_offset;

             
            fragmentable_offset = frag->ip_hdr_offset + frag->hlen;
            fragmentable_len = frag->data_len;
        }
        else {
            int pkt_end = fragmentable_offset + frag->offset + frag->data_len;
            if (pkt_end > (int)MAX_PAYLOAD_SIZE) {
                SCLogWarning(SC_ERR_REASSEMBLY, "Failed re-assemble "
                        "fragmented packet, exceeds size of packet buffer.");
                goto error_remove_tracker;
            }
            if (PacketCopyDataOffset(rp, fragmentable_offset + frag->offset + frag->ltrim,
                frag->pkt + frag->data_offset + frag->ltrim,
                frag->data_len - frag->ltrim) == -1) {
                goto error_remove_tracker;
            }
            if (frag->offset + frag->data_len > fragmentable_len)
                fragmentable_len = frag->offset + frag->data_len;
        }

        if (!frag->more_frags) {
            break;
        }
    }

    SCLogDebug("ip_hdr_offset %u, hlen %u, fragmentable_len %u",
            ip_hdr_offset, hlen, fragmentable_len);

    rp->ip4h = (IPV4Hdr *)(GET_PKT_DATA(rp) + ip_hdr_offset);
    int old = rp->ip4h->ip_len + rp->ip4h->ip_off;
    rp->ip4h->ip_len = htons(fragmentable_len + hlen);
    rp->ip4h->ip_off = 0;
    rp->ip4h->ip_csum = FixChecksum(rp->ip4h->ip_csum,
        old, rp->ip4h->ip_len + rp->ip4h->ip_off);
    SET_PKT_LEN(rp, ip_hdr_offset + hlen + fragmentable_len);

    tracker->remove = 1;
    DefragTrackerFreeFrags(tracker);
done:
    return rp;

error_remove_tracker:
    tracker->remove = 1;
    DefragTrackerFreeFrags(tracker);
    if (rp != NULL)
        PacketFreeOrRelease(rp);
    return NULL;
}