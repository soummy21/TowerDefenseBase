
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include <vector>


#pragma region ENTITY_H

#define ENTITY_H

class Entity: public sf::Drawable
{
public: 	
	struct PhysicsProperties
	{
		enum class Shape {
			Circle,
			Rectangle
		};
		
		enum class Type {
			Static,
			Dynamic
		};

		Shape shape;
		Type type;
		float radius;
		float width;
		float height;
		sf::Vector2f velocity;
		std::vector<Entity*> entitiesToIgnoreForCollision;
		
	};

	Entity(PhysicsProperties::Type type);
	~Entity() {};



public:

	void SetVelocity(const sf::Vector2f& velocity)
	{
		physicsProperties.velocity = velocity;
	}

	void SetTexture(const sf::Texture& texture)
	{
		sprite.setTexture(texture);
	}

	void SetScale(const sf::Vector2f& scale)
	{
		sprite.setScale(scale);
	}

	void SetPosition(const sf::Vector2f& position)
	{
		sprite.setPosition(position);
	}

	void SetSprite(const sf::Sprite& sp)
	{
		sprite = sp;
	}

	void SetCircle(float radius)
	{
		physicsProperties.shape = PhysicsProperties::Shape::Circle;
		physicsProperties.radius = radius;
	}

	void SetRectangle(float width, float height)
	{
		physicsProperties.shape = PhysicsProperties::Shape::Rectangle;
		physicsProperties.width = width;
		physicsProperties.height = height;
	}

	void AddEntityToIgnoreForCollision(Entity* entity)
	{
		physicsProperties.entitiesToIgnoreForCollision.push_back(entity);
	}

	bool ShouldIgnoreEntityForCollision(Entity* entity)
	{
		for (Entity* entityToIgnore : physicsProperties.entitiesToIgnoreForCollision)
		{
			if (entityToIgnore == entity)
			{
				return true;
			}
		}
		return false;
	}

	sf::Vector2f GetPosition() const
	{
		return sprite.getPosition();
	}

	void SetOrigin(const sf::Vector2f& origin)
	{
		sprite.setOrigin(origin);
	}

	void SetPathIndex(int index)
	{
		pathIndex = index;
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(sprite, states);
	}

	void Move(const sf::Vector2f& offset)
	{
		sprite.move(offset);
	}

	PhysicsProperties GetPhysicsProperties() const
	{
		return physicsProperties;
	}

	sf::Vector2i GetClosestCoordinates() const
	{
		int x = (int)GetPosition().x / 160;
		int y = (int)GetPosition().y / 160;
		return sf::Vector2i(x,y);
	}

	int GetPathIndex() 
	{
		return pathIndex;
	}

private: 
	PhysicsProperties physicsProperties;
	sf::Sprite sprite;

	//Enemy Data (Refector Later) 
	int pathIndex = 0;

};


#pragma endregion