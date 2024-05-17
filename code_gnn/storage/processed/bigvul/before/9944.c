void Part::slotTestArchive()
{
    TestJob *job = m_model->archive()->testArchive();
    if (!job) {
        return;
    }
    registerJob(job);
    connect(job, &KJob::result, this, &Part::slotTestingDone);
    job->start();
}
