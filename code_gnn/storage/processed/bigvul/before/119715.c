ResetScreenHandler::ResetScreenHandler()
    : BaseScreenHandler(kJsScreenPath),
      delegate_(NULL),
      show_on_init_(false),
      restart_required_(true),
      reboot_was_requested_(false),
      rollback_available_(false),
      weak_ptr_factory_(this) {
}
