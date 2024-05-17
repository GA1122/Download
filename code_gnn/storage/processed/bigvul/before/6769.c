static void trim_aio_cancel(BlockAIOCB *acb)
{
    TrimAIOCB *iocb = container_of(acb, TrimAIOCB, common);

     
    iocb->j = iocb->qiov->niov - 1;
    iocb->i = (iocb->qiov->iov[iocb->j].iov_len / 8) - 1;

    iocb->ret = -ECANCELED;

    if (iocb->aiocb) {
        blk_aio_cancel_async(iocb->aiocb);
        iocb->aiocb = NULL;
    }
}