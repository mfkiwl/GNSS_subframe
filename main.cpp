#include <iostream>
#include <cassert>
#include <map>
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
        template<size_t N_fields>
        consteval bool name_in_list(const Field (&in)[N_fields], const char* name) {
            bool found{false};
            for (auto field: in) {
                if (strcmp(field.name, name) == 0 ){
                    found = true;
                }
            }
            return found;
        }

        constexpr int strcmp(const char *a, const char *b) {
            while(*a && (*a == *b)) {
                ++a;
                ++b;
            }
            return static_cast<unsigned char>(*a) - static_cast<unsigned char>(*b);
        }
        template<size_t N_fields, const Field (&fields)[N_fields]>
        struct field_map {
             const Field &operator[](const char *name)  const {
                static_assert(name_in_list(fields, "m"));
                //const bool found{false};
                for (int i = 0; i < N_fields; i++) {
                    if (strcmp(fields[i].name, name) == 0) {
                        return fields[i];
                    }
                }
                throw std::out_of_range(name);
            }
        };
        typedef field_map<sizeof(GlonassLine1)/sizeof(GlonassLine1[0]), GlonassLine1> GlonassLine1Map;

        template<size_t N_bits, size_t N_fields, const Field (&fields)[N_fields]>
        struct Subframe {
            std::bitset<N_bits> data;
            bool get() {
                return data[0];
            }
        };
        typedef Subframe<76, sizeof(GlonassLine1)/sizeof(GlonassLine1[0]), GlonassLine1> Glonass_line_1;
    }
    using impl::Glonass_line_1;
    typedef impl::field_map<sizeof(impl::GlonassLine1)/sizeof(impl::GlonassLine1[0]), impl::GlonassLine1> Glonass_line_1_map;

}
int main() {

    //GNSS_data::Glonass_line_1 b;
    GNSS_data::Glonass_line_1_map m;
    std::cout << m["m"].loc.length << std::endl;
//    b.data[0] = 1;
//    std::cout << b.get() << std::endl;

}