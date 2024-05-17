base::TimeDelta GetPasswordRevealDuration() {
  return ViewsDelegate::GetInstance()
             ? ViewsDelegate::GetInstance()
                   ->GetTextfieldPasswordRevealDuration()
             : base::TimeDelta();
}
