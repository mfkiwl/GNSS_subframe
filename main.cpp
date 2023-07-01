#include <iostream>
#include <cassert>
#include <map>
#include <boost/dynamic_bitset.hpp>

// GNSS_data namespace contains all the classes related to GNSS data handling
namespace GNSS_data {
    // A struct to hold field information
    struct Field {
        size_t start, length;
    };

    typedef const std::map<std::string, Field> FieldMap;

    // Frame class is the basic class to hold and manipulate a frame of GNSS data
    class Frame {
        boost::dynamic_bitset<> data;
        FieldMap& fields;

        // A helper class to provide access to individual fields within a frame
        class FieldAccessor {
            boost::dynamic_bitset<>& parent;
            Field field;
        public:
            explicit FieldAccessor(boost::dynamic_bitset<>& parent, Field field) : parent(parent), field(field) {}

            // This operator returns a part of the parent bitset representing the field
            operator boost::dynamic_bitset<>() const {
                boost::dynamic_bitset<> result(field.length);
                for (size_t i = 0; i < field.length; ++i) {
                    result[i] = parent[field.start + i];
                }
                return result;
            }

            // This operator allows casting to an unsigned long
            operator unsigned long() const {
                boost::dynamic_bitset<> result = *this;
                return result.to_ulong();
            }

            // Overloaded assignment operator to handle assignment of a bitset to a field
            FieldAccessor& operator=(const boost::dynamic_bitset<>& inputBitset) {
                assert(inputBitset.size() == field.length);
                for (size_t i = 0; i < field.length; ++i) {
                    parent[field.start + i] = inputBitset[i];
                }
                return *this;
            }

            // Overloaded assignment operator to handle assignment of an unsigned long to a field
            FieldAccessor& operator=(const unsigned long inputNumber) {
                boost::dynamic_bitset<> bitset(field.length, inputNumber);
                return (*this) = bitset;
            }
        };

    public:
        explicit Frame(const uint size, const FieldMap& fields) : data{size}, fields{fields} {}

        // Overloaded [] operator to access individual fields by their names
        FieldAccessor operator[](const std::string& fieldName) {
            assert(this->fields.count(fieldName));
            return FieldAccessor(data, fields.at(fieldName));
        }
    };

    // GlonassLine is a specific type of Frame used in GLONASS systems
    class GlonassLine : public Frame {
        // 170 chips, 2 chips per bit, 8 hamming bits, 1 idle bit.
        static const int len{170/2-8-1};
    protected:
        explicit GlonassLine(FieldMap& fields) : Frame(len, fields) {}
    };

    const std::map<std::string, Field> GlonassLine2Fields{
            {"m",  {0, 4}},
            {"Bn", {4, 3}},
            {"P2", {7, 1}},
            {"tb", {8,7}},
            {"UNUSED1", {15,5}},
            {"yn_dot", {20, 24}},
            {"yn_dot_dot", {44, 5}},
            {"yn", {49, 27}}
    };

    // GlonassLine2 is a specific type of GlonassLine with a specific set of fields
    class GlonassLine2: public GlonassLine {
    public:
        explicit GlonassLine2(): GlonassLine(GlonassLine2Fields) {}
    };
}

int main() {
    using GNSS_data::GlonassLine2;
    GlonassLine2 g;
    g["Bn"] = 3;
    unsigned long Bn = g["Bn"];
    g["does not exist"] = 3;
    std::cout << Bn << std::endl;
}