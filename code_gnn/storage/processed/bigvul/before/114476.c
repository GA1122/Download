void ParseTestVideoData(FilePath::StringType data,
                        FilePath::StringType* file_name,
                        int* width, int* height,
                        int* num_frames,
                        int* num_NALUs,
                        int* min_fps_render,
                        int* min_fps_no_render,
                        int* profile) {
  std::vector<FilePath::StringType> elements;
  base::SplitString(data, ':', &elements);
  CHECK_GE(elements.size(), 1U) << data;
  CHECK_LE(elements.size(), 8U) << data;
  *file_name = elements[0];
  *width = *height = *num_frames = *num_NALUs = -1;
  *min_fps_render = *min_fps_no_render = -1;
  *profile = -1;
  if (!elements[1].empty())
    CHECK(base::StringToInt(elements[1], width));
  if (!elements[2].empty())
    CHECK(base::StringToInt(elements[2], height));
  if (!elements[3].empty())
    CHECK(base::StringToInt(elements[3], num_frames));
  if (!elements[4].empty())
    CHECK(base::StringToInt(elements[4], num_NALUs));
  if (!elements[5].empty())
    CHECK(base::StringToInt(elements[5], min_fps_render));
  if (!elements[6].empty())
    CHECK(base::StringToInt(elements[6], min_fps_no_render));
  if (!elements[7].empty())
    CHECK(base::StringToInt(elements[7], profile));
}
