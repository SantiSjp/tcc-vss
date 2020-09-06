#ifndef FIELD_H
#define FIELD_H

#include "DataTypes.h"

namespace vss{
    class Field{
    private:
        const lenght fieldLenght;
        const color fieldColor;

    public:
        Field(  const lenght mfieldLenght, 
                const color mfieldColor)
                    :fieldLenght(mfieldLenght),fieldColor(mfieldColor){};

        lenght getLenght() const {return fieldLenght;}
        color getColor() const {return fieldColor;}
    };
}


#endif // !FIELD_H