#include <iostream>
#include <cassert>
#include <boost/dynamic_bitset.hpp>
#include <bitset>

// GNSS_data namespace contains all the classes related to GNSS data handling
namespace GNSS_data {
    struct Field {
        const char *name;
        const struct {
            const size_t start, length;
        } loc;
    };
    namespace impl {
        constexpr Field GlonassLine1[]{
                {"m",           {0,  4}},
                {"UNUSED1",     {4,  2}},
                {"P1",          {6,  2}},
                {"tk",          {8,  12}},
                {"x_n_dot",     {20, 24}},
                {"x_n_dot_dot", {44, 5}},
                {"x_n",         {49, 27}},
        };

        constexpr Field GlonassLine2[]{
                {"m",          {0,  4}},
                {"Bn",         {4,  3}},
                {"P2",         {7,  1}},
                {"tb",         {8,  7}},
                {"UNUSED1",    {15, 5}},
                {"yn_dot",     {20, 24}},
                {"yn_dot_dot", {44, 5}},
                {"yn",         {49, 27}}
        };

        constexpr Field GlonassLine3[]{
                {"m",           {0,  4}},
                {"P3",          {4,  1}},
                {"gamma_n",     {5,  11}},
                {"UNUSED1",     {16, 1}},
                {"p",           {17, 2}},
                {"l_n",         {19, 1}},
                {"z_n_dot",     {20, 24}},
                {"z_n_dot_dot", {44, 5}},
                {"z_n",         {49, 27}}
        };

        constexpr Field GlonassLine4[]{
                {"m",           {0,  4}},
                {"tau_n",       {4,  22}},
                {"Delta_tau_n", {26, 5}},
                {"E_n",         {31, 5}},
                {"UNUSED1",     {36, 14}},
                {"P4",          {50, 1}},
                {"F_T",         {51, 4}},
                {"UNUSED2",     {55, 3}},
                {"N_T",         {58, 11}},
                {"n",           {69, 5}},
                {"M",           {74, 2}}
        };

        static constexpr int strcmp(const char *a, const char *b) {
            while (*a && (*a == *b)) {
                ++a;
                ++b;
            }
            return static_cast<unsigned char>(*a) - static_cast<unsigned char>(*b);
        }

        template<size_t N_fields>
        static constexpr const Field& get_field(const Field (&in)[N_fields], const char *name) {
            for (auto& field: in) {
                if (strcmp(field.name, name) == 0) {
                    return field;
                }
            }
            throw std::out_of_range(name);
        }

        template<size_t N_bits, size_t N_fields, const Field(& fields)[N_fields]>
        class Subframe {
        private:
            // A helper class to provide access to individual fields within a frame
            class FieldAccessor {
                std::bitset<N_bits>& parent;
            public:
                const Field& field;
                explicit constexpr FieldAccessor(std::bitset<N_bits>& parent, const Field& field) : parent(parent),
                                                                                          field(field) {}

                // This operator returns a part of the parent bitset representing the field
                operator boost::dynamic_bitset<>() const {
                    boost::dynamic_bitset<> result(field.loc.length);
                    for (size_t i = 0; i < field.loc.length; ++i) {
                        result[i] = parent[field.loc.start + i];
                    }
                    return result;
                }

                // This operator allows casting to an unsigned long
                operator unsigned long() const {
                    return static_cast<boost::dynamic_bitset<>>(*this).to_ulong();
                }

                // Overloaded assignment operator to handle assignment of a bitset to a field
                FieldAccessor &operator=(const boost::dynamic_bitset<>& inputBitset) {
                    assert(inputBitset.size() == field.loc.length);
                    for (size_t i = 0; i < field.loc.length; ++i) {
                        parent[field.loc.start + i] = inputBitset[i];
                    }
                    return *this;
                }

                // Overloaded assignment operator to handle assignment of an unsigned long to a field
                FieldAccessor &operator=(const unsigned long inputNumber) {
                    boost::dynamic_bitset<> bitset(field.loc.length, inputNumber);
                    return (*this) = bitset;
                }
            };
        public:

            FieldAccessor operator[](const char* fieldName) {
                return FieldAccessor(data, get_field(fields,fieldName));
            }

            // The name has to be externally linked to be used as a template parameter.
            template<const char* name>
            FieldAccessor get() {
                return FieldAccessor(data, get_field(fields, name));

            }

            template<uint field_index>
            FieldAccessor get() {
                static_assert(field_index<N_fields);
                return FieldAccessor(data, fields[field_index]);
            }
            std::bitset<N_bits> data;
            struct iterator {
                size_t i;
                std::bitset<N_bits>& parent;
                FieldAccessor operator*() {
                    return FieldAccessor(parent, fields[i]);
                }
                iterator& operator++() {
                    i++;
                    return *this;
                }

                bool operator!=(const iterator& other) const {
                    return this->i != other.i;
                }
            };

        public:
            iterator begin() {
                return iterator{0, data};
            }

            iterator end() {
                return iterator{N_fields, data};
            }
        };
    }

    typedef impl::Subframe<76, sizeof(impl::GlonassLine1)/sizeof(impl::GlonassLine1[0]), impl::GlonassLine1> GlonassLine1_subframe;
    typedef impl::Subframe<76, sizeof(impl::GlonassLine2)/sizeof(impl::GlonassLine2[0]), impl::GlonassLine2> GlonassLine2_subframe;
}
int main() {
    GNSS_data::GlonassLine1_subframe gs;
    gs["tk"] = 3;

    auto v= gs.get<6>();
    v = 23;
    std::cout << gs["m"];
    for (auto i: gs) {
        std::cout << i.field.name << ": " << i << "; ";
    }

    auto p = std::tuple{
            GNSS_data::GlonassLine1_subframe(),
            GNSS_data::GlonassLine2_subframe(),
    };
    //GNSS_data::Glonass_line_1 b;


//    b.data[0] = 1;
//    std::cout << b.get() << std::endl;

}