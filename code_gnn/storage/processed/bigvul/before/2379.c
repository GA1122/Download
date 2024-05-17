count_to_stopped(i_ctx_t *i_ctx_p, long mask)
{
    ref_stack_enum_t rsenum;
    uint scanned = 0;

    ref_stack_enum_begin(&rsenum, &e_stack);
    do {
        uint used = rsenum.size;
        es_ptr ep = rsenum.ptr + used - 1;
        uint count = used;

        for (; count; count--, ep--) {
            if (r_is_estack_mark(ep)) {
                if (estack_mark_index(ep) == es_stopped &&
                  (ep[2].value.intval & mask) != 0)
                    return scanned + (used - count + 1);
            }
        }
        scanned += used;
    } while (ref_stack_enum_next(&rsenum));
    return 0;
}
