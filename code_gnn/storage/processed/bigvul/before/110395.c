void SimulateInputEvent(PP_Instance instance, PP_Resource input_event) {
  PluginInstance* plugin_instance = host_globals->GetInstance(instance);
  if (!plugin_instance)
    return;

  EnterResource<PPB_InputEvent_API> enter(input_event, false);
  if (enter.failed())
    return;

  const InputEventData& input_event_data = enter.object()->GetInputEventData();
  plugin_instance->SimulateInputEvent(input_event_data);
}
