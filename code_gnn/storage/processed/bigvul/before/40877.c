static JSON_INLINE int isinf(double x) { return !isnan(x) && isnan(x - x); }
