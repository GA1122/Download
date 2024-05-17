int streamCompareID(streamID *a, streamID *b) {
    if (a->ms > b->ms) return 1;
    else if (a->ms < b->ms) return -1;
     
    else if (a->seq > b->seq) return 1;
    else if (a->seq < b->seq) return -1;
     
    return 0;
}
