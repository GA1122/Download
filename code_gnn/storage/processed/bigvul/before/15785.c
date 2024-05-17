static void process_ncq_command(AHCIState *s, int port, uint8_t *cmd_fis,
                                int slot)
{
    NCQFrame *ncq_fis = (NCQFrame*)cmd_fis;
    uint8_t tag = ncq_fis->tag >> 3;
    NCQTransferState *ncq_tfs = &s->dev[port].ncq_tfs[tag];

    if (ncq_tfs->used) {
         
        fprintf(stderr, "%s: tag %d already used\n", __FUNCTION__, tag);
        return;
    }

    ncq_tfs->used = 1;
    ncq_tfs->drive = &s->dev[port];
    ncq_tfs->slot = slot;
    ncq_tfs->lba = ((uint64_t)ncq_fis->lba5 << 40) |
                   ((uint64_t)ncq_fis->lba4 << 32) |
                   ((uint64_t)ncq_fis->lba3 << 24) |
                   ((uint64_t)ncq_fis->lba2 << 16) |
                   ((uint64_t)ncq_fis->lba1 << 8) |
                   (uint64_t)ncq_fis->lba0;

     
    ncq_tfs->sector_count = ((uint16_t)ncq_fis->sector_count_high << 8) |
                                ncq_fis->sector_count_low;

    DPRINTF(port, "NCQ transfer LBA from %"PRId64" to %"PRId64", "
            "drive max %"PRId64"\n",
            ncq_tfs->lba, ncq_tfs->lba + ncq_tfs->sector_count - 2,
            s->dev[port].port.ifs[0].nb_sectors - 1);

    ahci_populate_sglist(&s->dev[port], &ncq_tfs->sglist, 0);
    ncq_tfs->tag = tag;

    switch(ncq_fis->command) {
        case READ_FPDMA_QUEUED:
            DPRINTF(port, "NCQ reading %d sectors from LBA %"PRId64", "
                    "tag %d\n",
                    ncq_tfs->sector_count-1, ncq_tfs->lba, ncq_tfs->tag);

            DPRINTF(port, "tag %d aio read %"PRId64"\n",
                    ncq_tfs->tag, ncq_tfs->lba);

            dma_acct_start(ncq_tfs->drive->port.ifs[0].bs, &ncq_tfs->acct,
                           &ncq_tfs->sglist, BDRV_ACCT_READ);
            ncq_tfs->aiocb = dma_bdrv_read(ncq_tfs->drive->port.ifs[0].bs,
                                           &ncq_tfs->sglist, ncq_tfs->lba,
                                           ncq_cb, ncq_tfs);
            break;
        case WRITE_FPDMA_QUEUED:
            DPRINTF(port, "NCQ writing %d sectors to LBA %"PRId64", tag %d\n",
                    ncq_tfs->sector_count-1, ncq_tfs->lba, ncq_tfs->tag);

            DPRINTF(port, "tag %d aio write %"PRId64"\n",
                    ncq_tfs->tag, ncq_tfs->lba);

            dma_acct_start(ncq_tfs->drive->port.ifs[0].bs, &ncq_tfs->acct,
                           &ncq_tfs->sglist, BDRV_ACCT_WRITE);
            ncq_tfs->aiocb = dma_bdrv_write(ncq_tfs->drive->port.ifs[0].bs,
                                            &ncq_tfs->sglist, ncq_tfs->lba,
                                            ncq_cb, ncq_tfs);
            break;
        default:
            DPRINTF(port, "error: tried to process non-NCQ command as NCQ\n");
            qemu_sglist_destroy(&ncq_tfs->sglist);
            break;
    }
}
