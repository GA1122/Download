job_cancel (Device *device)
{
  g_return_if_fail (device->priv->job != NULL);

  device->priv->job->was_cancelled = TRUE;

   
  kill (device->priv->job->pid, SIGTERM);
}
