bool PDFiumEngine::IsPointInEditableFormTextArea(FPDF_PAGE page,
                                                 double page_x,
                                                 double page_y,
                                                 int form_type) {
  FPDF_ANNOTATION annot =
      FPDFAnnot_GetFormFieldAtPoint(form_, page, page_x, page_y);
  DCHECK(annot);

  int flags = FPDFAnnot_GetFormFieldFlags(page, annot);
  bool is_editable_form_text_area =
      CheckIfEditableFormTextArea(flags, form_type);
  FPDFPage_CloseAnnot(annot);
  return is_editable_form_text_area;
}
