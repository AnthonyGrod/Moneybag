#ifndef MONEYBAG_MONEYBAG_H
#define MONEYBAG_MONEYBAG_H

#include <iostream>
#include <cstddef>
#include <compare>

inline constexpr int DENIER = 1;
inline constexpr int SOLIDUS = 12;
inline constexpr int LIVRE = 240;

class Moneybag {

public:
    using coin_number_t = uint64_t;
    constexpr Moneybag (coin_number_t livres, coin_number_t soliduses, coin_number_t denieres)
            : livre (livres)
            , solidus (soliduses)
            , denier (denieres)
    {}

    inline constexpr coin_number_t livre_number () const;

    inline constexpr coin_number_t solidus_number () const;

    inline constexpr coin_number_t denier_number () const;

    inline constexpr Moneybag& operator+=(const Moneybag& moneybag);

    inline constexpr Moneybag& operator-=(const Moneybag& moneybag);

    inline constexpr Moneybag& operator*=(size_t multi);

    inline constexpr const Moneybag operator*(size_t multi) const;

    inline constexpr friend const Moneybag operator*(size_t multi, const Moneybag& moneybag);

    inline constexpr const Moneybag operator+(const Moneybag& moneybag) const;

    inline constexpr const Moneybag operator-(const Moneybag& moneybag) const;

    inline constexpr std::partial_ordering operator<=>(const Moneybag& moneybag) const;

    inline constexpr bool operator==(const Moneybag& moneybag) const;

    inline constexpr explicit operator bool() const;

    inline friend std::ostream& operator<<(std::ostream &s, const Moneybag &moneybag);

private:
    coin_number_t livre;
    coin_number_t solidus;
    coin_number_t denier;
};

inline constexpr Moneybag::coin_number_t Moneybag::livre_number () const {
    return livre;
}

inline constexpr Moneybag::coin_number_t Moneybag::solidus_number () const {
    return solidus;
}

inline constexpr Moneybag::coin_number_t Moneybag::denier_number () const {
    return denier;
}

inline constexpr Moneybag& Moneybag::operator+=(const Moneybag& moneybag) {
    if (UINT64_MAX - moneybag.livre < livre || UINT64_MAX - moneybag.solidus < solidus || UINT64_MAX - moneybag.denier < denier) {
        throw std::out_of_range ("Numbers can not be added because the sum out of range");
    }
    else {
        livre += moneybag.livre;
        solidus += moneybag.solidus;
        denier += moneybag.denier;
    }
    return *this;
}

inline constexpr Moneybag& Moneybag::operator-=(const Moneybag& moneybag) {
    if (moneybag.livre > livre || moneybag.solidus > solidus || moneybag.denier > denier) {
        throw std::out_of_range ("Numbers cannot be subtracted because the difference is less than zero");
    }
    else {
        livre -= moneybag.livre;
        solidus -= moneybag.solidus;
        denier -= moneybag.denier;
    }
    return *this;
}

inline constexpr Moneybag& Moneybag::operator*=(const size_t multi) {
    if (UINT64_MAX / multi < livre || UINT64_MAX / multi < solidus || UINT64_MAX / multi < denier) {
        throw std::out_of_range ("Numbers can not be multiplied because the multiplication is out of range");
    }
    else {
        livre *= multi;
        solidus *= multi;
        denier *= multi;
    }
    return *this;
}

inline constexpr const Moneybag Moneybag::operator*(const size_t multi) const {
    return Moneybag(*this) *= multi;
}

inline constexpr const Moneybag operator*(const size_t multi, const Moneybag& moneybag) {
    return moneybag * multi;
}

inline constexpr const Moneybag Moneybag::operator+(const Moneybag& moneybag) const {
    return (Moneybag(*this) += moneybag);
}

inline constexpr const Moneybag Moneybag::operator-(const Moneybag& moneybag) const {
    return Moneybag(*this) -= moneybag;
}

inline constexpr Moneybag::operator bool() const {
    return !(livre == 0 && solidus == 0 && denier == 0);
}

inline constexpr std::partial_ordering Moneybag::operator<=>(const Moneybag& moneybag) const{
    if (livre == moneybag.livre && solidus == moneybag.solidus && denier == moneybag.denier) {
        return std::partial_ordering::equivalent;
    }
    if (livre <= moneybag.livre && solidus <= moneybag.solidus && denier <= moneybag.denier) {
        return std::partial_ordering::less;
    }
    if (livre >= moneybag.livre && solidus >= moneybag.solidus && denier >= moneybag.denier) {
        return std::partial_ordering::greater;
    }
    return std::partial_ordering::unordered;
}

inline constexpr bool Moneybag::operator==(const Moneybag& moneybag) const {
    return (livre == moneybag.livre && solidus == moneybag.solidus && denier == moneybag.denier);
}


inline std::ostream& operator<<(std::ostream &s, const Moneybag &moneybag) {
    return s << "("<<
             moneybag.livre << " livr" << (moneybag.livre == 1 ? "" : "es" ) << ", " <<
             moneybag.solidus << " solidus" << (moneybag.solidus == 1 ? "" : "es") << ", " <<
             moneybag.denier << " denier" << (moneybag.denier == 1 ? "" : "s") << ")";
}

inline const constinit Moneybag Livre(1, 0, 0);

inline const constinit Moneybag Solidus(0, 1, 0);

inline const constinit Moneybag Denier(0, 0, 1);


class Value {

public:
    constexpr Value (const Moneybag &moneybag)
            : value_sum (moneybag.denier_number() * DENIER + moneybag.solidus_number() * SOLIDUS + moneybag.livre_number() * LIVRE)
    {}

    constexpr Value (Moneybag::coin_number_t coin_amount)
            : value_sum ((__uint128_t) coin_amount)
    {}

    constexpr Value ()
            : value_sum (0)
    {}

    inline explicit operator std::string() const;

    inline constexpr bool operator==(const Value& other_value) const;

    inline constexpr const Value& operator=(const Value& other_value);

    inline constexpr std::strong_ordering operator<=>(const Value& other_value) const;

    inline constexpr std::strong_ordering operator<=>(const __uint128_t other_value) const;

private:
    __uint128_t value_sum;
};

inline Value::operator std::string() const {
    __uint128_t n = value_sum;
    if (n == 0) {
        return "0";
    }

    std::string result = std::string(40, '0');
    int index = result.size() - 1;
    while (n != 0) {
        result[index] = "0123456789"[n % 10];
        n /= 10;
        index--;
    }
    return result.erase(0, result.find_first_not_of('0'));;
}

inline constexpr bool Value::operator==(const Value& other_value) const {
    return (value_sum == other_value.value_sum);
}

inline constexpr const Value& Value::operator=(const Value& other_value) {
    value_sum = other_value.value_sum;
    return *this;
}

inline constexpr std::strong_ordering Value::operator<=>(const Value& other_value) const {
    if (value_sum == other_value.value_sum) {
        return std::strong_ordering::equivalent;
    }
    if (value_sum > other_value.value_sum) {
        return std::strong_ordering::greater;
    }
    return std::strong_ordering::less;
}

inline constexpr std::strong_ordering Value::operator<=>(const __uint128_t other_value) const {
    if (value_sum == other_value) {
        return std::strong_ordering::equivalent;
    }
    if (value_sum > other_value) {
        return std::strong_ordering::greater;
    }
    return std::strong_ordering::less;
}


#endif
