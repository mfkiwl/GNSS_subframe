#include <iostream>
#include <cassert>
#include <boost/dynamic_bitset.hpp>

namespace GNSS_data {
    struct field {
        size_t start, length;
    };

    class frame {
        boost::dynamic_bitset<> data;
    public:
        explicit frame(const int size) : data(size) {}

        boost::dynamic_bitset<> operator[](field f) const {
            boost::dynamic_bitset<> result(f.length);
            for (size_t i = 0; i < f.length; ++i) {
                result[i] = data[f.start + i];
            }
            return result;
        }

        void set(field f, const boost::dynamic_bitset<> &value) {
            assert (f.length == value.size());
            for (int i = 0; i < f.length; i++) {
                data[f.start + i] = value[i];
            }
        }

        void set(const field f, const unsigned long value) {
            boost::dynamic_bitset<> b_value(f.length, value);
            set(f, b_value);
        }

    };

    class Glonass_line : public frame {
        static const int len{85-8-1};
    public:
        Glonass_line() : frame(len) {}
    };

    class Glonass_line2: public Glonass_line {
    public:
        static constexpr field m{0,4};
        static constexpr field Bn{4,3};
        static constexpr field P2{7,1};
        static constexpr field tb{8,7};
        static constexpr field UNUSED1{15,5};
        static constexpr field yn_dot{20, 24};
        static constexpr field yn_dot_dot{44, 5};
        static constexpr field yn{49, 27};
//        static constexpr field KX{75, 8}; // Hamming code is not meant to be here
    };
}

#if __cplusplus < 201703L
constexpr GNSS_data::field GNSS_data::Glonass_line2::Bn;
#endif
int main() {
    using GNSS_data::Glonass_line2;
    Glonass_line2 g;
    std::cout << Glonass_line2::Bn.start;
    g.set(Glonass_line2::Bn, 3);
    unsigned long x = g[GNSS_data::Glonass_line2::Bn].to_ulong();
    std::cout << x;
}