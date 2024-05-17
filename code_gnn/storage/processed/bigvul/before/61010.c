abort_job (CommonJob *job)
{
     
    g_clear_object (&job->undo_info);

    g_cancellable_cancel (job->cancellable);
}
