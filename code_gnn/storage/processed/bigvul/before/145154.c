FontRenderParamsOnIO& GetFontRenderParamsOnIO() {
  static base::NoDestructor<FontRenderParamsOnIO> instance;
  return *instance;
}
