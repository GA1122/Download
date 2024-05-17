static int is_ncq(uint8_t ata_cmd)
{
     
    switch (ata_cmd) {
    case READ_FPDMA_QUEUED:
    case WRITE_FPDMA_QUEUED:
    case NCQ_NON_DATA:
    case RECEIVE_FPDMA_QUEUED:
    case SEND_FPDMA_QUEUED:
        return 1;
    default:
        return 0;
    }
}
