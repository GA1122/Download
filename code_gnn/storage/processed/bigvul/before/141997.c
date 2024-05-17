  void IssueOnQuery(int query_id) {
    const FormData form;
    FormFieldData field;
    field.is_focusable = true;
    field.should_autocomplete = true;

    external_delegate_->OnQuery(query_id, form, field, gfx::RectF());
  }
