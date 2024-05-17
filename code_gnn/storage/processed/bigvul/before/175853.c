 RandomSpi(int min, int max) : mMin(min) {
        std::mt19937 rnd = std::mt19937(std::random_device()());
        mNext = std::uniform_int_distribution<>(1, INT_MAX)(rnd);
        mSize = max - min + 1;
        mCount = mSize;
 }
