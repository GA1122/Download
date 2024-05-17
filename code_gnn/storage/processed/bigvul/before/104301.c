bool MockWebRTCPeerConnectionHandler::initialize(const WebRTCConfiguration&, const WebMediaConstraints& constraints)
{
    WebVector<WebString> mandatoryConstraintNames;
    constraints.getMandatoryConstraintNames(mandatoryConstraintNames);
    if (mandatoryConstraintNames.size()) {
        for (size_t i = 0; i < mandatoryConstraintNames.size(); ++i) {
            if (!isSupportedConstraint(mandatoryConstraintNames[i]))
                return false;
            WebString value;
            constraints.getMandatoryConstraintValue(mandatoryConstraintNames[i], value);
            if (value != "1")
                return false;
        }
    }

    WebVector<WebString> optionalConstraintNames;
    constraints.getOptionalConstraintNames(optionalConstraintNames);
    if (optionalConstraintNames.size()) {
        for (size_t i = 0; i < optionalConstraintNames.size(); ++i) {
            if (!isValidConstraint(optionalConstraintNames[i]))
                return false;
            WebString value;
            constraints.getOptionalConstraintValue(optionalConstraintNames[i], value);
            if (value != "0")
                return false;
        }
    }

    return true;
}
