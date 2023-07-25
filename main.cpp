#include <cassert>
#include <boost/dynamic_bitset.hpp>
#include <span>

// GNSS_data namespace contains all the classes related to GNSS data handling
namespace GNSS_data {
    struct Field {
        const char *name;
        const struct {
            const size_t start, length;
        } loc;
    };
    namespace impl {
    class span: public std::span<const Field> {
        /**
         * An expansion of std::span to support constexpr operator[].
         */
    public:
        template <size_t _ArrayExtent>
        constexpr span(const Field (&f)[_ArrayExtent]):
        std::span<const Field>(f)
        {}


        constexpr const Field& operator[](const char* name) const {
            for (auto& f: *this) {
                if (span::strcmp(f.name, name)==0) {
                    return f;
                }
            }
            throw std::out_of_range(name);
        }

    private:
        constexpr static int strcmp(const char* a, const char *b) {
            while(*a && (*a == *b)) {
                a++;
                b++;
            }
            return static_cast<unsigned char>(*a) - static_cast<unsigned char>(*b);
        }

    };

        constexpr Field GlonassLine1_arr[]{
                {"m",           {0,  4}},
                {"UNUSED1",     {4,  2}},
                {"P1",          {6,  2}},
                {"tk",          {8,  12}},
                {"x_n_dot",     {20, 24}},
                {"x_n_dot_dot", {44, 5}},
                {"x_n",         {49, 27}},
        };

        constexpr span GlonassLine1{GlonassLine1_arr};

        constexpr Field GlonassLine2_arr[]{
                {"m",          {0,  4}},
                {"Bn",         {4,  3}},
                {"P2",         {7,  1}},
                {"tb",         {8,  7}},
                {"UNUSED1",    {15, 5}},
                {"yn_dot",     {20, 24}},
                {"yn_dot_dot", {44, 5}},
                {"yn",         {49, 27}}
        };

        constexpr span GlonassLine2{GlonassLine2_arr};

        constexpr Field GlonassLine3_arr[]{
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

        constexpr span GlonassLine3{GlonassLine3_arr};

        constexpr Field GlonassLine4_arr[]{
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

        constexpr span GlonassLine4{GlonassLine4_arr};


        class Subframe {
        private:
            boost::dynamic_bitset<> data;
            virtual constexpr span get_fields()=0;
            class FieldAccessor {
                boost::dynamic_bitset<>& parent;
            public:
                const Field& field;
                explicit constexpr FieldAccessor(boost::dynamic_bitset<>& parent, const Field& field) :
                    parent(parent),
                    field(field) {
                }

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

            constexpr FieldAccessor operator[](const char* fieldName) {

                return FieldAccessor(data,
                                     get_fields()[fieldName]);

            }


            struct iterator {
                size_t i;
                boost::dynamic_bitset<>& parent;
                std::span<const Field> fields;

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

                return iterator{0, data, get_fields()};
            }

            iterator end() {
                return iterator{get_fields().size(), data, get_fields()};
            }
            Subframe(size_t size): data(size) {}
        };
    }
    class GlonassLine: public impl::Subframe {
    public:
        GlonassLine(): impl::Subframe{76} {}
    };

    class GlonassLine1: public GlonassLine {
        constexpr impl::span get_fields() override {
            return impl::GlonassLine1;
        }
    };
}

#include <iostream>
void test() {
    constexpr auto m  { GNSS_data::impl::GlonassLine1["m"]};
    GNSS_data::GlonassLine1 gs;
    gs["tk"] = 3;


    gs["m"] = 4;
//    for (auto i: {"tk", "m"}) {
//        std::cout << i << ": " << gs[i] << std::endl;
//    }

    for (auto i: gs) {
        std::cout << i.field.name << ": " << i << "; ";
    }
}

int main() {
    test();
}