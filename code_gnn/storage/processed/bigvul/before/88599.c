char pdf_get_object_status(
    const pdf_t *pdf,
    int          xref_idx,
    int          entry_idx)
{
    int                 i, curr_ver;
    const xref_t       *prev_xref;
    const xref_entry_t *prev, *curr;

    curr = &pdf->xrefs[xref_idx].entries[entry_idx];
    curr_ver = pdf->xrefs[xref_idx].version;

    if (curr_ver == 1)
      return 'A';

     
    if (curr->f_or_n == 'f')
      return 'D';

     
    prev_xref = NULL;
    for (i=xref_idx; i>-1; --i)
      if (pdf->xrefs[i].version < curr_ver)
      {
          prev_xref = &pdf->xrefs[i];
          break;
      }

    if (!prev_xref)
      return '?';

     
    prev = NULL;
    for (i=0; i<prev_xref->n_entries; ++i)
      if (prev_xref->entries[i].obj_id == curr->obj_id)
      {
          prev = &prev_xref->entries[i];
          break;
      }

     
    if (!prev || ((prev->f_or_n == 'f') && (curr->f_or_n == 'n')))
      return 'A';

     
    else if (prev->offset != curr->offset)
      return 'M';
    
    return '?';
}
