static void ncq_finish(NCQTransferState *ncq_tfs)
{
     
    if (!(ncq_tfs->drive->port_regs.scr_err & (1 << ncq_tfs->tag))) {
        ncq_tfs->drive->finished |= (1 << ncq_tfs->tag);
    }

    ahci_write_fis_sdb(ncq_tfs->drive->hba, ncq_tfs);

    DPRINTF(ncq_tfs->drive->port_no, "NCQ transfer tag %d finished\n",
            ncq_tfs->tag);

    block_acct_done(blk_get_stats(ncq_tfs->drive->port.ifs[0].blk),
                    &ncq_tfs->acct);
    qemu_sglist_destroy(&ncq_tfs->sglist);
    ncq_tfs->used = 0;
}
