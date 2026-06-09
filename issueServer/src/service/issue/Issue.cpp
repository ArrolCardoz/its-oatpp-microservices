#include "Issue.hpp"

#include <string>

std::string Issue::priorityToString(Issue::Priority p) const {
    switch (p) {
        case Issue::Priority::LOW:
            return "Low";
            break;
        case Issue::Priority::MEDIUM:
            return "Medium";
            break;
        case Issue::Priority::HIGH:
            return "High";
            break;
        case Issue::Priority::UNDEFINED:
        default:
            return "Undefined";
    }
}
