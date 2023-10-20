#pragma once
#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <iostream>
#include <random>

class Calculator {
public:
    static int GetRandomIndex(int min, int max) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(gen);
    }
};

#endif // CALCULATOR_H
