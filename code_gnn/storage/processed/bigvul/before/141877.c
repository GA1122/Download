void AutofillPopupBaseView::OnMouseExited(const ui::MouseEvent& event) {
  if (!delegate_ || !delegate_->HasSelection())
    return;

  base::ThreadTaskRunnerHandle::Get()->PostTask(
      FROM_HERE, base::BindOnce(&AutofillPopupBaseView::ClearSelection,
                                weak_ptr_factory_.GetWeakPtr()));
}
