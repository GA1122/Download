   virtual void SendHandwritingStroke(const HandwritingStroke& stroke) {
//   virtual void SendHandwritingStroke(
//       const input_method::HandwritingStroke& stroke) {
      if (!initialized_successfully_)
        return;
    chromeos::SendHandwritingStroke(input_method_status_connection_, stroke);
//     ibus_controller_->SendHandwritingStroke(stroke);
    }