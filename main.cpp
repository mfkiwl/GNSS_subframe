#include <iostream>
#include <cassert>
#include <boost/dynamic_bitset.hpp>

namespace GNSS_data {
    struct field {
        size_t start, length;
    };

    class frame {
        boost::dynamic_bitset<> data;
        class bitset_field_accessor {
            boost::dynamic_bitset<>& parent;
            field f;
        public:
            explicit bitset_field_accessor(boost::dynamic_bitset<>& parent,
                                           field f):
                    parent(parent), f(f) {}
            operator boost::dynamic_bitset<>() const {
                boost::dynamic_bitset<> result(f.length);
                for (size_t i = 0; i < f.length; ++i) {
                    result[i] = parent[f.start + i];
                }
                return result;
            }
            operator unsigned long() const {
                boost::dynamic_bitset<> result = *this;
                return result.to_ulong();
            }

            bitset_field_accessor& operator=(const boost::dynamic_bitset<>& in)  {
                assert (in.size() == f.length);
                for (size_t i = 0; i < f.length; ++i) {
                    parent[f.start + i] = in[i];
                }
                return *this;
            }

            bitset_field_accessor& operator=(const unsigned long in){
                boost::dynamic_bitset<> b(f.length, in);
                return (*this) = b;
            }
        };
    public:
        explicit frame(const int size) : data(size) {}
        bitset_field_accessor operator[](field f)  {
            return bitset_field_accessor(data, f);
        }
    };

    class Glonass_line : public frame {
        // 170 chips, 2 chips per bit, 8 hamming bits, 1 idle bit.
        static const int len{170/2-8-1};
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
    g[g.Bn] = 3;
    unsigned long Bn = g[g.Bn];
    std::cout << Bn << std::endl;
}