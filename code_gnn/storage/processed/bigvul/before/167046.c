 void MetalayerMode::UpdateState() {
   if (enabled() && !selectable())
     delegate()->DisableTool(GetToolId());

  if (!loading())
    Shell::Get()->toast_manager()->Cancel(kToastId);

  UpdateView();
}
