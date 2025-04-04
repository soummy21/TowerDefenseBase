#pragma once

#include <SFML/Graphics.hpp>

namespace Math
{
    //Returns the length of the given vector
    float GetVectorLength(const sf::Vector2f& refVector)
    {
        return sqrt(refVector.x * refVector.x + refVector.y * refVector.y);
    }

    //Returns the normalized vector for the given vector
    sf::Vector2f NormalizeVector(const sf::Vector2f& refVector)
    {
        float length = GetVectorLength(refVector);

        // Avoid division by zero or very small values
        if (length < 0.0001f)
        {
            return sf::Vector2f(0, 0);
        }

        return sf::Vector2f(refVector.x / length, refVector.y / length);
    }

}
