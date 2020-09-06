#ifndef FIELD_H
#define FIELD_H

#include "DataTypes.h"

namespace vss{
    class Field{
    private:
        lenght fieldLenght;
        color fieldColor;

    public:
        Field(  lenght mfieldLenght, 
                color mfieldColor)
                    :fieldLenght(mfieldLenght),fieldColor(mfieldColor){};

        lenght getLenght(){return fieldLenght;}
        color getColor(){return fieldColor;}
    };
}


#endif // !FIELD_H