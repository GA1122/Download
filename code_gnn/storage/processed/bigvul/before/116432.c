static bool shareSameAxis(const WebTransformOperation* from, const WebTransformOperation* to, double& axisX, double& axisY, double& axisZ, double& angleFrom)
{
    if (isIdentity(from) && isIdentity(to))
        return false;

    if (isIdentity(from) && !isIdentity(to)) {
        axisX = to->rotate.axis.x;
        axisY = to->rotate.axis.y;
        axisZ = to->rotate.axis.z;
        angleFrom = 0;
        return true;
    }

    if (!isIdentity(from) && isIdentity(to)) {
        axisX = from->rotate.axis.x;
        axisY = from->rotate.axis.y;
        axisZ = from->rotate.axis.z;
        angleFrom = from->rotate.angle;
        return true;
    }

    double length2 = from->rotate.axis.x * from->rotate.axis.x + from->rotate.axis.y * from->rotate.axis.y + from->rotate.axis.z * from->rotate.axis.z;
    double otherLength2 = to->rotate.axis.x * to->rotate.axis.x + to->rotate.axis.y * to->rotate.axis.y + to->rotate.axis.z * to->rotate.axis.z;

    if (length2 <= EPSILON || otherLength2 <= EPSILON)
        return false;

    double dot = to->rotate.axis.x * from->rotate.axis.x + to->rotate.axis.y * from->rotate.axis.y + to->rotate.axis.z * from->rotate.axis.z;
    double error = fabs(1.0 - (dot * dot) / (length2 * otherLength2));
    bool result = error < EPSILON;
    if (result) {
        axisX = to->rotate.axis.x;
        axisY = to->rotate.axis.y;
        axisZ = to->rotate.axis.z;
        angleFrom = dot > 0 ? from->rotate.angle : -from->rotate.angle;
    }
    return result;
}
