void Shell::RotateFocus(Direction direction) {
  focus_cycler_->RotateFocus(direction == FORWARD ? FocusCycler::FORWARD
                                                  : FocusCycler::BACKWARD);
}
