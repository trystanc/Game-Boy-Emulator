#include "RegisterPair.h"
#include "../types.h"
#include <iostream>
RegisterPair& RegisterPair::operator= (u16 num){
    lo = static_cast<u8>(num);
    hi = static_cast<u8>(num >> 8);
    return *this;
}

RegisterPair& RegisterPair::operator= (RegisterPair rp){
    lo = rp.lo;
    hi = rp.hi;
    return *this;
}

u16 operator+(RegisterPair rp, u16 num){
    return static_cast<u16>(rp) + num;
}

u16 operator+(RegisterPair rp1, RegisterPair rp2){
    return static_cast<u16>(rp1) + static_cast<u16>(rp2);
}

u16 operator+(u16 num, RegisterPair rp){
    return static_cast<u16>(rp) + num;
}

RegisterPair& RegisterPair::operator++(){
    *this = static_cast<u16>(*this) + static_cast<u16>(1);
    return *this;
}



u16 operator-(RegisterPair rp, u16 num){
    return static_cast<u16>(rp) - num;
}

u16 operator-(RegisterPair rp1, RegisterPair rp2){
    return static_cast<u16>(rp1) - static_cast<u16>(rp2);
}

u16 operator-(u16 num, RegisterPair rp){
    return num - static_cast<u16>(rp);
}

RegisterPair& RegisterPair::operator--(){
    *this = static_cast<u16>(*this) - static_cast<u16>(1);
    return *this;
}

std::ostream& operator<<(std::ostream& os, const RegisterPair& rp) {
    os << std::hex << static_cast<u16>(rp);
    return os;
}

