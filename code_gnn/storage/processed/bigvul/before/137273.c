void Textfield::RevealPasswordChar(int index) {
  GetRenderText()->SetObscuredRevealIndex(index);
  SchedulePaint();

  if (index != -1) {
    password_reveal_timer_.Start(
        FROM_HERE, GetPasswordRevealDuration(),
        base::Bind(&Textfield::RevealPasswordChar,
                   weak_ptr_factory_.GetWeakPtr(), -1));
  }
}
