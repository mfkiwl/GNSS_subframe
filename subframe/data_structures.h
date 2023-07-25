//
// Created by veksler on 25.07.23.
//
#pragma once
#include <span>
#include <stdexcept>


namespace GNSS_data {
    struct Field {
        const char *fieldName;
        const struct {
            const size_t startPosition, length;
        } location;
    };
    namespace implementation {
        class span : public std::span<const Field> {
            /**
             * std::span<const Field> with a constexpr operator[] to access fields by name.
             */
        public:
            template<size_t ArraySize>
            constexpr span(const Field (&fieldsArray)[ArraySize]) :
                    std::span<const Field>(fieldsArray) {}

            // Operator[] to return field with the given name
            constexpr const Field &operator[](const char *name) const {
                for (auto &field: *this) {
                    if (strcmp(field.fieldName, name) == 0) {
                        return field;
                    }
                }
                throw std::out_of_range(name);
            }

        private:
            constexpr static int strcmp(const char *a, const char *b) {
                /**
                 * Library strcmp is not constexpr
                 */
                while (*a && (*a == *b)) {
                    a++;
                    b++;
                }
                return static_cast<unsigned char>(*a) - static_cast<unsigned char>(*b);
            }
        };

    }
}