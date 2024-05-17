 untrusted_launcher_response_callback (GtkDialog                 *dialog,
                                       int                        response_id,
                                       ActivateParametersDesktop *parameters)
 {
     GdkScreen *screen;
     char *uri;
     GFile *file;
  
      switch (response_id)
      {
        case RESPONSE_RUN:
//         case GTK_RESPONSE_OK:
          {
//             file = nautilus_file_get_location (parameters->file);
// 
//              
//             nautilus_file_set_metadata (parameters->file, NAUTILUS_METADATA_KEY_DESKTOP_FILE_TRUSTED,
//                                         NULL,
//                                         "yes");
// 
//             nautilus_file_mark_desktop_file_executable (file,
//                                                         parameters->parent_window,
//                                                         TRUE,
//                                                         NULL, NULL);
// 
//              
//             nautilus_file_invalidate_all_attributes (parameters->file);
// 
              screen = gtk_widget_get_screen (GTK_WIDGET (parameters->parent_window));
              uri = nautilus_file_get_uri (parameters->file);
              DEBUG ("Launching untrusted launcher %s", uri);
              nautilus_launch_desktop_file (screen, uri, NULL,
                                            parameters->parent_window);
              g_free (uri);
        }
        break;
        case RESPONSE_MARK_TRUSTED:
        {
            file = nautilus_file_get_location (parameters->file);
            nautilus_file_mark_desktop_file_trusted (file,
                                                     parameters->parent_window,
                                                     TRUE,
                                                     NULL, NULL);
              g_object_unref (file);
          }
          break;
 
         default:
         {
              
         }
         break;
     }
 
     gtk_widget_destroy (GTK_WIDGET (dialog));
     activate_parameters_desktop_free (parameters);
 }