static void mptsas_write_sequence_write(MPTSASState *s, uint32_t val)
{
     
    if (s->diagnostic & MPI_DIAG_DRWE) {
        goto disable;
    }

    switch (s->diagnostic_idx) {
    case 0:
        if ((val & MPI_WRSEQ_KEY_VALUE_MASK) != MPI_WRSEQ_1ST_KEY_VALUE) {
            goto disable;
        }
        break;
    case 1:
        if ((val & MPI_WRSEQ_KEY_VALUE_MASK) != MPI_WRSEQ_2ND_KEY_VALUE) {
            goto disable;
        }
        break;
    case 2:
        if ((val & MPI_WRSEQ_KEY_VALUE_MASK) != MPI_WRSEQ_3RD_KEY_VALUE) {
            goto disable;
        }
        break;
    case 3:
        if ((val & MPI_WRSEQ_KEY_VALUE_MASK) != MPI_WRSEQ_4TH_KEY_VALUE) {
            goto disable;
        }
        break;
    case 4:
        if ((val & MPI_WRSEQ_KEY_VALUE_MASK) != MPI_WRSEQ_5TH_KEY_VALUE) {
            goto disable;
        }
         
        s->diagnostic |= MPI_DIAG_DRWE;
        break;
    }
    s->diagnostic_idx++;
    return;

disable:
    s->diagnostic &= ~MPI_DIAG_DRWE;
    s->diagnostic_idx = 0;
}
