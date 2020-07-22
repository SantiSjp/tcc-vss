#include "gtest/gtest.h"
#include "../../../lib/include/Utils.h"

namespace {

    TEST(Uppercase, TestUppercase) {
        ASSERT_EQ ("TESTE", Utils::uppercase("teste"));
    }

    

}