static int xt_has_compatible_appcontext_timerQueue(void)
{
  int is_compatible;
  XtIntervalId id;
  TimerEventRec *tq, *tq_probe;

   
  tq = XtNew(TimerEventRec);
  XtFree((char *)tq);
  tq_probe = XtNew(TimerEventRec);
  XtFree((char *)tq_probe);
  if (tq != tq_probe)
	return 0;

  id = XtAppAddTimeOut(x_app_context, 0,
					   xt_dummy_timeout_cb,
					   GUINT_TO_POINTER(0xdeadbeef));

  tq = x_app_context->timerQueue;
  is_compatible = tq == tq_probe
	&& tq->app == x_app_context
	&& tq->te_proc == xt_dummy_timeout_cb
	&& tq->te_closure == GUINT_TO_POINTER(0xdeadbeef)
	;

  XtRemoveTimeOut(id);
  return is_compatible;
}
