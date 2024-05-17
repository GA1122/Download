bool PrintRenderFrameHelper::RenderPreviewPage(
    int page_number,
    const PrintMsg_Print_Params& print_params) {
  std::unique_ptr<PdfMetafileSkia> draft_metafile;
  PdfMetafileSkia* initial_render_metafile = print_preview_context_.metafile();
  if (print_preview_context_.IsModifiable() && is_print_ready_metafile_sent_) {
    draft_metafile =
        base::MakeUnique<PdfMetafileSkia>(print_params.printed_doc_type);
    initial_render_metafile = draft_metafile.get();
  }

  base::TimeTicks begin_time = base::TimeTicks::Now();
  PrintPageInternal(print_params, page_number,
                    print_preview_context_.total_page_count(),
                    print_preview_context_.prepared_frame(),
                    initial_render_metafile, nullptr, nullptr);
  print_preview_context_.RenderedPreviewPage(base::TimeTicks::Now() -
                                             begin_time);
  if (draft_metafile.get()) {
    draft_metafile->FinishDocument();
  } else if (print_preview_context_.IsModifiable() &&
             print_preview_context_.generate_draft_pages()) {
    DCHECK(!draft_metafile.get());
    draft_metafile =
        print_preview_context_.metafile()->GetMetafileForCurrentPage(
            print_params.printed_doc_type);
  }
  return PreviewPageRendered(page_number, draft_metafile.get());
}
