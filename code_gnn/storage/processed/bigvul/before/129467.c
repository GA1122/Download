static bool PrecisionMeetsSpecForHighpFloat(GLint rangeMin,
                                            GLint rangeMax,
                                            GLint precision) {
  return (rangeMin >= 62) && (rangeMax >= 62) && (precision >= 16);
}
