g_NPP_HandleEvent(NPP instance, NPEvent *event)
{
  if (instance == NULL)
	return false;

  if (plugin_funcs.event == NULL)
	return false;

  if (event == NULL)
	return false;

  D(bugiI("NPP_HandleEvent instance=%p, event=%p [%s]\n", instance, event, string_of_NPEvent_type(event->type)));
  int16_t ret = plugin_funcs.event(instance, event);
  D(bugiD("NPP_HandleEvent return: %d\n", ret));

   
  if (event->type == GraphicsExpose)
	gdk_flush();

  return ret;
}