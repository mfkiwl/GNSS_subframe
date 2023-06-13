#include <iostream>
#include <cassert>
#include <boost/dynamic_bitset.hpp>

namespace GNSS_data {
    struct field {
        size_t start, length;
    };

    class frame {
        boost::dynamic_bitset<> data;

        class field_accessor {
            boost::dynamic_bitset<>& parent;
            field f;
        public:
            explicit field_accessor(boost::dynamic_bitset<>& parent,
                                    field f):
                    parent(parent), f(f) {}
            operator boost::dynamic_bitset<>() const {
                boost::dynamic_bitset<> result(f.length);
                for (size_t i = 0; i < f.length; ++i) {
                    result[i] = parent[f.start + i];
                }
                return result;
            }
            /**
             * @throw std::overflow_error
             */
            operator unsigned long() const {
                boost::dynamic_bitset<> result = *this;
                return result.to_ulong();
            }

            field_accessor& operator=(const boost::dynamic_bitset<>& in)  {
                assert (in.size() == f.length);
                for (size_t i = 0; i < f.length; ++i) {
                    parent[f.start + i] = in[i];
                }
                return *this;
            }

            field_accessor& operator=(const unsigned long in){
                boost::dynamic_bitset<> b(f.length, in);
                return (*this) = b;
            }
        };
    public:
        explicit frame(const int size) : data(size) {}
        std::map<const std::string, field> fields;
        field_accessor operator[](const std::string& fieldName) {
            assert (this->fields.count(fieldName));
            return field_accessor(data, fields[fieldName]);
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
        explicit Glonass_line2() {
         this->fields.insert({
                    {"m",  {0, 4}},
                    {"Bn", {4, 3}},
                    {"P2", {7, 1}},
                    {"tb", {8,7}},
                    {"UNUSED1", {15,5}},
                    {"yn_dot", {20, 24}},
                    {"yn_dot_dot", {44, 5}},
                    {"yn", {49, 27}}
            });
        }
    };
}

int main() {
    using GNSS_data::Glonass_line2;
    Glonass_line2 g;
    g["Bn"] = 3;
    unsigned long Bn = g["Bn"];
    std::cout << Bn << std::endl;
}