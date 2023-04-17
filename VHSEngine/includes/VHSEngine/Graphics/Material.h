#pragma once
#include "VHSEngine/CoreMinimal.h"

class Material {

public:
	Material();
	~Material();

	TexturePtr BaseColour;

	void Draw(ShaderPtr Shader);

};