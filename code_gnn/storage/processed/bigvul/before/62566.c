frf15_print(netdissect_options *ndo,
            const u_char *p, u_int length)
{
    uint16_t sequence_num, flags;

    if (length < 2)
        goto trunc;
    ND_TCHECK2(*p, 2);

    flags = p[0]&MFR_BEC_MASK;
    sequence_num = (p[0]&0x1e)<<7 | p[1];

    ND_PRINT((ndo, "FRF.15, seq 0x%03x, Flags [%s],%s Fragmentation, length %u",
           sequence_num,
           bittok2str(frf_flag_values,"none",flags),
           p[0]&FR_FRF15_FRAGTYPE ? "Interface" : "End-to-End",
           length));

 
    return;

trunc:
    ND_PRINT((ndo, "[|frf.15]"));
}
