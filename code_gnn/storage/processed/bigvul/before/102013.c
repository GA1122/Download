    PrintDialogGtkInterface* (*create_dialog_func)(
        PrintingContextCairo* context)) {
  DCHECK(create_dialog_func);
  DCHECK(!create_dialog_func_);
  create_dialog_func_ = create_dialog_func;
}
