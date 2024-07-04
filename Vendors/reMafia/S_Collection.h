#pragma once

template<class T>
class S_Collection {
public:
    int NumElements() {
        return ((int(__thiscall*)(S_Collection*))(0x47BBB0))(this);
    }

    T& GetElement(int index) {
        return ((T(__thiscall*)(S_Collection*, int))(0x47BE10))(this, index);
    }

    T& operator[](int iIndex) {
        return GetElement(iIndex);
    }

private:
    int unk;
    T* start;
    T* end;
};
