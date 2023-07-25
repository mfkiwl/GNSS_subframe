//
// Created by veksler on 25.07.23.
//
#pragma once
#include "format_definitions.h"
#include "data_structures.h"

namespace GNSS_data {
    namespace implementation {
        // Class to expand std::span to support constexpr operator[]

        // A class representing a Subframe in GNSS data
        class Subframe {
        private:
            boost::dynamic_bitset<> data;
            virtual constexpr span get_fields() = 0;

            // FieldAccessor class to manage individual fields within the bitset
            class FieldAccessor {
                boost::dynamic_bitset<>& parentBitset;
            public:
                const Field& field;
                explicit constexpr FieldAccessor(boost::dynamic_bitset<>& parent, const Field& field) :
                        parentBitset(parent), field(field) {}

                // Returns a part of the parent bitset representing the field
                operator boost::dynamic_bitset<>() const {
                    boost::dynamic_bitset<> result(field.location.length);
                    for (size_t i = 0; i < field.location.length; ++i) {
                        result[i] = parentBitset[field.location.startPosition + i];
                    }
                    return result;
                }

                // Allows casting to an unsigned long
                operator unsigned long() const {
                    return static_cast<boost::dynamic_bitset<>>(*this).to_ulong();
                }

                // Overloaded assignment operator to handle assignment of a bitset to a field
                FieldAccessor &operator=(const boost::dynamic_bitset<>& inputBitset) {
                    assert(inputBitset.size() == field.location.length);
                    for (size_t i = 0; i < field.location.length; ++i) {
                        parentBitset[field.location.startPosition + i] = inputBitset[i];
                    }
                    return *this;
                }

                // Overloaded assignment operator to handle assignment of an unsigned long to a field
                FieldAccessor &operator=(const unsigned long inputNumber) {
                    boost::dynamic_bitset<> bitset(field.location.length, inputNumber);
                    return (*this) = bitset;
                }
            };
        public:
            constexpr FieldAccessor operator[](const char* fieldName) {
                return FieldAccessor(data, get_fields()[fieldName]);
            }

            // Iterator class for iterating through fields in the subframe
            struct iterator {
                size_t index;
                boost::dynamic_bitset<>& parentBitset;
                std::span<const Field> fields;

                FieldAccessor operator*() {
                    return FieldAccessor(parentBitset, fields[index]);
                }
                iterator& operator++() {
                    index++;
                    return *this;
                }

                bool operator!=(const iterator& other) const {
                    return this->index != other.index;
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
        class GlonassLine: public implementation::Subframe {
        public:
            GlonassLine(): implementation::Subframe{76} {}
        };


    }

    class GlonassLine1: public implementation::GlonassLine {
        constexpr implementation::span get_fields() override {
            return fGlonassLine1;
        }
    };
}