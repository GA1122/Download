gpgsm_set_status_handler (void *engine, engine_status_handler_t fnc,
			  void *fnc_value)
{
  engine_gpgsm_t gpgsm = engine;

  gpgsm->status.fnc = fnc;
  gpgsm->status.fnc_value = fnc_value;
}
