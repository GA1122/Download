gimp_test_load_image (Gimp  *gimp,
                      GFile *file)
{
  GimpPlugInProcedure *proc;
  GimpImage           *image;
  GimpPDBStatusType    unused;

  proc = gimp_plug_in_manager_file_procedure_find (gimp->plug_in_manager,
                                                   GIMP_FILE_PROCEDURE_GROUP_OPEN,
                                                   file,
                                                   NULL  );
  image = file_open_image (gimp,
                           gimp_get_user_context (gimp),
                           NULL  ,
                           file,
                           file,
                           FALSE  ,
                           proc,
                           GIMP_RUN_NONINTERACTIVE,
                           &unused  ,
                           NULL  ,
                           NULL  );

  return image;
}
