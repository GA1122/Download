void ComputeBuiltInPlugins(std::vector<content::PepperPluginInfo>* plugins) {
  static bool skip_pdf_file_check = false;
  FilePath path;
  if (PathService::Get(chrome::FILE_PDF_PLUGIN, &path)) {
    if (skip_pdf_file_check || file_util::PathExists(path)) {
      content::PepperPluginInfo pdf;
      pdf.path = path;
      pdf.name = kPDFPluginName;
      webkit::WebPluginMimeType pdf_mime_type(kPDFPluginMimeType,
                                              kPDFPluginExtension,
                                              kPDFPluginDescription);
      webkit::WebPluginMimeType print_preview_pdf_mime_type(
          kPDFPluginPrintPreviewMimeType,
          kPDFPluginExtension,
          kPDFPluginDescription);
      pdf.mime_types.push_back(pdf_mime_type);
      pdf.mime_types.push_back(print_preview_pdf_mime_type);
      plugins->push_back(pdf);

      skip_pdf_file_check = true;
    }
  }

  static bool skip_nacl_file_check = false;
  if (PathService::Get(chrome::FILE_NACL_PLUGIN, &path)) {
    if (skip_nacl_file_check || file_util::PathExists(path)) {
      content::PepperPluginInfo nacl;
      nacl.path = path;
      nacl.name = kNaClPluginName;
      webkit::WebPluginMimeType nacl_mime_type(kNaClPluginMimeType,
                                               kNaClPluginExtension,
                                               kNaClPluginDescription);
      nacl.mime_types.push_back(nacl_mime_type);
      plugins->push_back(nacl);

      skip_nacl_file_check = true;
    }
  }

  static bool skip_o3d_file_check = false;
  if (PathService::Get(chrome::FILE_O3D_PLUGIN, &path)) {
    if (skip_o3d_file_check || file_util::PathExists(path)) {
      content::PepperPluginInfo o3d;
      o3d.path = path;
      o3d.name = kO3DPluginName;
      o3d.is_out_of_process = true;
      o3d.is_sandboxed = false;
      webkit::WebPluginMimeType o3d_mime_type(kO3DPluginMimeType,
                                              kO3DPluginExtension,
                                              kO3DPluginDescription);
      o3d.mime_types.push_back(o3d_mime_type);
      plugins->push_back(o3d);

      skip_o3d_file_check = true;
    }
  }

  static bool skip_gtalk_file_check = false;
  if (PathService::Get(chrome::FILE_GTALK_PLUGIN, &path)) {
    if (skip_gtalk_file_check || file_util::PathExists(path)) {
      content::PepperPluginInfo gtalk;
      gtalk.path = path;
      gtalk.name = kGTalkPluginName;
      gtalk.is_out_of_process = true;
      gtalk.is_sandboxed = false;
      webkit::WebPluginMimeType gtalk_mime_type(kGTalkPluginMimeType,
                                                kGTalkPluginExtension,
                                                kGTalkPluginDescription);
      gtalk.mime_types.push_back(gtalk_mime_type);
      plugins->push_back(gtalk);

      skip_gtalk_file_check = true;
    }
  }

#if defined(ENABLE_REMOTING)
  content::PepperPluginInfo info;
  info.is_internal = true;
  info.name = kRemotingViewerPluginName;
  info.path = FilePath(kRemotingViewerPluginPath);
  webkit::WebPluginMimeType remoting_mime_type(
      kRemotingViewerPluginMimeType,
      std::string(),
      std::string());
  info.mime_types.push_back(remoting_mime_type);
  webkit::WebPluginMimeType old_remoting_mime_type(
      kRemotingViewerPluginOldMimeType,
      std::string(),
      std::string());
  info.mime_types.push_back(old_remoting_mime_type);
  info.internal_entry_points.get_interface = remoting::PPP_GetInterface;
  info.internal_entry_points.initialize_module =
      remoting::PPP_InitializeModule;
  info.internal_entry_points.shutdown_module = remoting::PPP_ShutdownModule;

  plugins->push_back(info);
#endif
}
