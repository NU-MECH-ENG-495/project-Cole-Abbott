#include "first_project/header1.h"
#include <string>

namespace first_project {

bool are_strings_equal(const std::string& str1, const std::string& str2) {
    if (&str1 == &str2) {  // If they're the same string object
        return true;
    }
    return str1 == str2;
}

}  // namespace first_project
 