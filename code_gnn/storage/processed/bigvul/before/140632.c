pp::Var ModalDialog(const pp::Instance* instance,
                    const std::string& type,
                    const std::string& message,
                    const std::string& default_answer) {
  const PPB_Instance_Private* interface =
      reinterpret_cast<const PPB_Instance_Private*>(
          pp::Module::Get()->GetBrowserInterface(
              PPB_INSTANCE_PRIVATE_INTERFACE));
  pp::VarPrivate window(pp::PASS_REF,
      interface->GetWindowObject(instance->pp_instance()));
  if (default_answer.empty())
    return window.Call(type, message);
  else
    return window.Call(type, message, default_answer);
}
