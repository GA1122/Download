bool PDFiumEngine::IsPointInEditableFormTextArea(FPDF_PAGE page,
                                                 double page_x,
                                                 double page_y,
                                                 int form_type) {
#if defined(PDF_ENABLE_XFA)
  if (IS_XFA_FORMFIELD(form_type))
    return form_type == FPDF_FORMFIELD_XFA_TEXTFIELD ||
           form_type == FPDF_FORMFIELD_XFA_COMBOBOX;
#endif   

  FPDF_ANNOTATION annot =
      FPDFAnnot_GetFormFieldAtPoint(form_, page, page_x, page_y);
  if (!annot)
    return false;

  int flags = FPDFAnnot_GetFormFieldFlags(page, annot);
  bool is_editable_form_text_area =
      CheckIfEditableFormTextArea(flags, form_type);
  FPDFPage_CloseAnnot(annot);
  return is_editable_form_text_area;
}
