#pragma once

#define TILE_OPTION_H
#include <SFML/Graphics.hpp>

class TileOption : public sf::Drawable
{
public: 
	enum class TileType
	{
		Null = -1,
		Aesthetic,
		Spawn,
		End,
		Path,
	};

	TileOption(TileType type);

	void SetSprite(const sf::Sprite& sprite) { tileSprite = sprite; }
	const sf::Sprite& GetSprite() { return tileSprite; }
	
	void setPosition(const sf::Vector2f& postion) { tileSprite.setPosition(postion); }

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(tileSprite, states);
	}

	const TileType& GetTileType() { return tileType; }

private: 
	sf::Sprite tileSprite;
	TileType tileType;
};

