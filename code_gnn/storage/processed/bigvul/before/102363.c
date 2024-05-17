void ExtensionHelper::OnExtensionDeliverMessage(int target_id,
                                                const std::string& message) {
  MiscellaneousBindings::DeliverMessage(
      extension_dispatcher_->v8_context_set().GetAll(),
      target_id,
      message,
      render_view());
}
