#include "gtest/gtest.h"
#include "Vision/src/Field.h"

namespace{
    using namespace vss;

    TEST(FieldTest, CreateField){
        lenght len = {1,2};
        color cor = {3,4};
        Field field(len,cor);

        for(int i = 0; i < field.getColor().size(); i++){
            ASSERT_EQ(field.getLenght()[i],len[i]);
            ASSERT_EQ(field.getColor()[i], cor[i]);
        }
    }

}