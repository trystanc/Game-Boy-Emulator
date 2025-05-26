#ifndef REGISTERPAIR_H_INCLUDED
#define REGISTERPAIR_H_INCLUDED
#include "../types.h"
#include <iostream>

//Defines pair of registers which holds reference to two
// 8-bit registers. Contains reference to the two lower bits
// and a bunch of function overloads to treat it like a 16-bit
//integer which modifies the registers it holds a reference to.
class RegisterPair{
    public:
    u8& hi;
    u8& lo;


   RegisterPair(u8& high, u8& low) : hi{high}, lo{low}{}

    //allows implicit conversion to it's 16 bit value
    operator u16() const {return (static_cast<u16>(hi) << 8) | lo;}
    //can assign a 16-bit value to register pair using =
    RegisterPair& operator= (u16 num);

    friend u16 operator+(RegisterPair rp, u16 num);
    friend u16 operator+(u16 num, RegisterPair rp);
    friend u16 operator+(RegisterPair rp1, RegisterPair rp2);
    RegisterPair& operator++();



    friend u16 operator-(RegisterPair rp, u16 num);
    friend u16 operator-(u16 num, RegisterPair rp);
    friend u16 operator-(RegisterPair rp1, RegisterPair rp2);
    RegisterPair& operator--();

    friend std::ostream& operator<<(std::ostream& os, const RegisterPair& rp);
};


#endif // REGISTERPAIR_H_INCLUDED
