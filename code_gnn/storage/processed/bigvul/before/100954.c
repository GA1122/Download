static inline TransformationMatrix createProjectionMatrix(const IntSize& size, bool flip)
{
    const float near = 9999999;
    const float far = -99999;

    return TransformationMatrix(2.0 / float(size.width()), 0, 0, 0,
                                0, (flip ? -2.0 : 2.0) / float(size.height()), 0, 0,
                                0, 0, -2.f / (far - near), 0,
                                -1, flip ? 1 : -1, -(far + near) / (far - near), 1);
}
