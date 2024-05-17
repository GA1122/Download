load_gimp_2_6_file (gconstpointer data)
{
  Gimp      *gimp = GIMP (data);
  GimpImage *image;
  gchar     *filename;
  GFile     *file;

  filename = g_build_filename (g_getenv ("GIMP_TESTING_ABS_TOP_SRCDIR"),
                               "app/tests/files/gimp-2-6-file.xcf",
                               NULL);
  file = g_file_new_for_path (filename);
  g_free (filename);

  image = gimp_test_load_image (gimp, file);

   
  gimp_assert_mainimage (image,
                         FALSE  ,
                         FALSE  ,
                         FALSE  );
}
