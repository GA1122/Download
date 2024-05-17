static int round(double x) {
 if (x < 0)
 return static_cast<int>(ceil(x - 0.5));
 else
 return static_cast<int>(floor(x + 0.5));
}
