uiserver_reset (void *engine)
{
  engine_uiserver_t uiserver = engine;

   
  return uiserver_assuan_simple_command (uiserver->assuan_ctx, "RESET", NULL, NULL);
}
