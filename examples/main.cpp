
#include "json/string.hpp"

using json::String;

int main() {
    String str{"Dupa"};

    str.insert(3, "test");
}
