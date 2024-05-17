gpgsm_reset (void *engine)
{
  engine_gpgsm_t gpgsm = engine;

   
  return (gpgsm->assuan_ctx
          ? gpgsm_assuan_simple_command (gpgsm->assuan_ctx, "RESET",
                                         NULL, NULL)
          : 0);
}
