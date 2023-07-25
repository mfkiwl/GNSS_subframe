//
// Created by veksler on 25.07.23.
//
#pragma once
#include "data_structures.h"
#include <boost/dynamic_bitset.hpp>


namespace GNSS_data {
namespace implementation{
        constexpr Field GlonassLine1_arr[]{
            {"m",           {0,  4}},
            {"UNUSED1",     {4,  2}},
            {"P1",          {6,  2}},
            {"tk",          {8,  12}},
            {"x_n_dot",     {20, 24}},
            {"x_n_dot_dot", {44, 5}},
            {"x_n",         {49, 27}},
            };

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

    }
    constexpr implementation::span fGlonassLine1{implementation::GlonassLine1_arr};
    constexpr implementation::span fGlonassLine2{implementation::GlonassLine2_arr};
    constexpr implementation::span fGlonassLine3{implementation::GlonassLine3_arr};
    constexpr implementation::span fGlonassLine4{implementation::GlonassLine4_arr};
}