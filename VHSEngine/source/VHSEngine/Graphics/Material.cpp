#include "VHSEngine/Graphics/Material.h"
#include "VHSEngine/Graphics/Texture.h"
#include "VHSEngine/Graphics/ShaderProgram.h"
#include "VHSEngine/Game.h"

Material::Material()
{
	//make sure the base colour can be set to engine default
	if (Game::GetGameInstance().GetDefaultEngineTexture() != nullptr) {
		BaseColour.TextureV3 = Game::GetGameInstance().GetDefaultEngineTexture();
		SpecularColour.TextureV3 = Game::GetGameInstance().GetDefaultEngineTexture();
		EmissiveColour.TextureV3 = Game::GetGameInstance().GetDefaultEngineTexture();
	}
	else {
		BaseColour.TextureV3 = SpecularColour.TextureV3 = EmissiveColour.TextureV3 = nullptr;
	}

	EmissiveColour.MultiplierV3 = Vector3(0.0f);
	Shininess = 1.0f;
}

Material::~Material()
{
	BaseColour.TextureV3 = SpecularColour.TextureV3 = EmissiveColour.TextureV3 = nullptr;
}

void Material::Draw(ShaderPtr Shader)
{
	//base colour to shader
	if (BaseColour.TextureV3 != nullptr) {
		//activate the texture for slot 0
		BaseColour.TextureV3->ActivateTexture(0);
		//telling the shader to get the texture colour from slot 0
		Shader->SetInt("ImportMat.TColour", 0);
		//binfd the texture to open gl
		BaseColour.TextureV3->BindTexture();
	}

	Shader->SetVector3("ImportMat.MColour", BaseColour.MultiplierV3);

	//base colour to shader
	if (SpecularColour.TextureV3 != nullptr) {
		//activate the texture for slot 1
		SpecularColour.TextureV3->ActivateTexture(1);
		//telling the shader to get the texture colour from slot 0
		Shader->SetInt("ImportMat.TSpecular", 1);
		//binfd the texture to open gl
		SpecularColour.TextureV3->BindTexture();
	}

	Shader->SetVector3("ImportMat.MSpecular", SpecularColour.MultiplierV3);

	//base colour to shader
	if (EmissiveColour.TextureV3 != nullptr) {
		//activate the texture for slot 2
		EmissiveColour.TextureV3->ActivateTexture(2);
		//telling the shader to get the texture colour from slot 0
		Shader->SetInt("ImportMat.TEmissive", 2);
		//binfd the texture to open gl
		EmissiveColour.TextureV3->BindTexture();
	}

	Shader->SetVector3("ImportMat.MEmissive", EmissiveColour.MultiplierV3);

	//set the shininess in the shader
	Shader->SetFloat("ImportMat.Shininess", Shininess);
}
