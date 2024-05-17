static void bubbleSort(double array[])
{
  for (int j = 0; j < 3; ++j) {
    int kk = j;
    for (int k = j + 1; k < 4; ++k) {
      if (array[k] < array[kk]) {
        kk = k;
      }
    }
    double tmp = array[j];
    array[j] = array[kk];
    array[kk] = tmp;
  }
}
