e1000e_tx_ring_init(E1000ECore *core, E1000E_TxRing *txr, int idx)
{
    static const E1000E_RingInfo i[E1000E_NUM_QUEUES] = {
        { TDBAH,  TDBAL,  TDLEN,  TDH,  TDT, 0 },
        { TDBAH1, TDBAL1, TDLEN1, TDH1, TDT1, 1 }
    };

    assert(idx < ARRAY_SIZE(i));

    txr->i     = &i[idx];
    txr->tx    = &core->tx[idx];
}
