void RenderThread::OnExtensionMessageInvoke(const std::string& function_name,
                                            const ListValue& args) {
  RendererExtensionBindings::Invoke(function_name, args, NULL);
}
