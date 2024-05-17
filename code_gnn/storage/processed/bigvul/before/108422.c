void ChromeRenderMessageFilter::OnExtensionGenerateUniqueID(int* unique_id) {
  static int next_unique_id = 1;
  *unique_id = next_unique_id++;
}
