  explicit TextfieldDestroyerController(views::Textfield* target)
      : target_(target) {
    target_->set_controller(this);
  }
