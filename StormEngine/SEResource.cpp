/******************************************************************************//**
 * \file			SEResource.cpp
 * \author 			Rommel Sim (100%)
 * \par    			zhenpengrommel.sim@digipen.edu
 *
 * \brief			Resource Manager
 *
 * \date   			January 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#pragma once
#include "pch.h"
#include "SEResource.h"
#include "SEAudio.h"
#include <ConsoleColor.h>

/**
 * Basic interface for resource class.
 */
class IResource
{
public:
	std::filesystem::path m_folderpath;
	std::filesystem::path m_filename;
	std::string m_name;

public:
	const std::string& getFolderpath() { return m_folderpath.generic_string(); }			// returns Folder path
	const std::string& getFilename() { return m_filename.generic_string(); }				// returns the name of the file 
	const std::string& getName() { return m_name; }											// return name
};

/**
 * Texture Resource Class
 */
class TextureResource : public IResource
{
public:
	//GLuint ID;
	//unsigned char* data;
	//int width, height, chn;
	std::unique_ptr<SETexture> data;

	TextureResource(std::filesystem::path path)
	{
		m_folderpath = path.generic_string().substr(0, path.generic_string().find_last_of("/"));
		m_filename = path.generic_string();
		m_name = path.filename().string();
		data = std::make_unique<SETexture>();
	}
	~TextureResource()
	{
		/*if (data)
			stbi_image_free(data);*/
	}
};

/**
 * Font Resource Class.
 */
class FontResource : public IResource
{
public:

	/*struct Character
	{
		unsigned int TextureID;
		glm::ivec2 Size;
		glm::ivec2 Bearing;
		unsigned int Advance;
	};*/
	FT_Library ft;
	FT_Face face;
	const unsigned char MAX_CHAR_LOAD = 128;
	SEFont font;
	//using Font = std::unordered_map<char, Character>; Font font;		// Package given to GraphicsSys to render per char


	FontResource(std::filesystem::path path) : ft{}, face{}, font{}
	{
		m_folderpath = path.generic_string().substr(0, path.generic_string().find_last_of("/"));
		m_filename = path.generic_string();
		m_name = path.filename().string();

	}
	~FontResource()
	{

	}
};

class AudioResource : public IResource
{
public:
	std::unique_ptr<SEAudio> pAudio;

public:
	AudioResource(std::filesystem::path path)
	{
		pAudio = std::make_unique<SEAudio>();
		m_folderpath = path.generic_string().substr(0, path.generic_string().find_last_of("/"));
		m_filename = path.generic_string();
		m_name = path.filename().string();
	}
};

static GLuint TextureID;
std::unordered_map<std::string, TextureResource*> Texture_Resources{};
std::unordered_map<std::string, FontResource*> Font_Resources{};
std::unordered_map<std::string, AudioResource*> BGM_Resources{};
std::unordered_map<std::string, AudioResource*> SFX_Resources{};


void SELoadAllFonts(std::filesystem::path path)
{
	using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
	for (const auto& dirEntry : recursive_directory_iterator(path))
		SELoadFont(dirEntry);
}

void SELoadAllBgm(std::filesystem::path path)
{
	using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
	for (const auto& dirEntry : recursive_directory_iterator(path))
		SELoadSound(AUDIOTYPE::BGM, dirEntry);
}

void SELoadAllSfx(std::filesystem::path path)
{
	using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
	for (const auto& dirEntry : recursive_directory_iterator(path))
		SELoadSound(AUDIOTYPE::SFX, dirEntry);
}

void SELoadTexture(std::filesystem::path path)
{
	auto search = Texture_Resources.find(path.filename().string());

	if (!(search != Texture_Resources.end()))
	{
		std::unique_ptr<TextureResource> texture_tmp = std::make_unique<TextureResource>(path);
		if (texture_tmp)
		{
			std::string name = texture_tmp->getName();
			//texture_tmp->ID = TextureID++;
			texture_tmp->data->ID = TextureID++;
			texture_tmp->data->texture = stbi_load(path.string().c_str(), &texture_tmp->data->width, &texture_tmp->data->height, &texture_tmp->data->chn, 0);
			if (texture_tmp->data->texture)
			{
				glGenTextures(1, &texture_tmp->data->ID);
				glBindTexture(GL_TEXTURE_2D, texture_tmp->data->ID);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

				//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				//glTextureSubImage2D(texture_tmp->data->ID, 0, 0, 0, texture_tmp->data->width, texture_tmp->data->height, GL_RGBA, GL_UNSIGNED_BYTE, texture_tmp->data->texture);
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_tmp->data->width, texture_tmp->data->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_tmp->data->texture);
				stbi_image_free(texture_tmp->data->texture);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
			{
#if _DEBUG
				std::cout << red << "fail to load " << path.filename().string() << white << "\n";
#else
				std::string out{ "failed to load " + path.filename().string() + "\n" };
				SEWrite(out);
#endif
				exit(EXIT_FAILURE);
			}
			Texture_Resources.emplace(std::make_pair(name, texture_tmp.release()));
#if _DEBUG
			std::cout << name << " loaded\n";
#else
			std::string tmp{ name + "loaded\n" };
			SEWrite(tmp);
#endif
		}
	}
}

GLuint SEGetTexture(std::filesystem::path _name)
{
	auto search = Texture_Resources.find(_name.filename().string());
	if (search != Texture_Resources.end())
		return Texture_Resources.at(_name.filename().string())->data->ID;
	return 0;
}

void SELoadFont(std::filesystem::path path)
{
	auto search = Font_Resources.find(path.filename().string());
	if (!(search != Font_Resources.end()))
	{

		std::unique_ptr<FontResource> font_tmp = std::make_unique<FontResource>(path);
		FT_Library ft;
		FT_Face face;

		if (FT_Init_FreeType(&ft))
		{
#if _DEBUG
			std::cout << red << "ERROR::FREETYPE: Could not init FreeType Library" << white << std::endl;
#else
			SEWrite("ERROR::FREETYPE: Could not init FreeType Library\n");
#endif
		}

		// check my container of fonts
		// if my container does not have this font type, add it.

		if (FT_New_Face(ft, path.string().c_str(), 0, &face))
		{
#if _DEBUG
			std::cout << red << "ERROR::FREETYPE: Failed to load font" << white << std::endl;
#else
			std::string temp{ "ERROR::FREETYPE: Failed to load font" + path.filename().string() + "\n" };
			SEWrite(temp);
#endif
		}
		// set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, 48);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		unsigned int texture;

		for (unsigned char c = 0; c < 128; c++)
		{
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
#if _DEBUG
				std::cout << red << "ERROR::FREETYTPE: Failed to load Glyph" << white << std::endl;
#endif
				continue;
			}
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);

			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			/*SE_Font::Character character =
			{
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)
			};*/

			SECharacter character =
			{
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)

			};
			font_tmp->font.emplace(std::make_pair(c, character));
		}
		Font_Resources.emplace(path.filename().string(), font_tmp.release());
#if _DEBUG
		std::cout << path.filename().string() << " loaded\n";
#else
		std::string tmp{ path.filename().string() + "loaded\n" };
		SEWrite(tmp);
#endif
	}
}

void SELoadSound(AUDIOTYPE TrackType, std::filesystem::path path)
{
	auto name = path.filename().string();
	auto search_bgm = BGM_Resources.find(name);
	auto search_sfx = SFX_Resources.find(name);
	if (!(search_bgm != BGM_Resources.end()) || !(search_sfx != SFX_Resources.end()))
	{
		switch (TrackType)
		{
		case AUDIOTYPE::BGM:
		{
			// if not found, add audio into container.
			std::unique_ptr<AudioResource> tmp = std::make_unique<AudioResource>(path);
			tmp->pAudio->isBGM = true;
			tmp->pAudio->isSFX = false;
			auto audioHandler = SEGetAudioHandle();
			auto res = audioHandler->system->createSound(path.string().c_str(), FMOD_LOOP_NORMAL, 0, &tmp->pAudio->track); (void)res;
			BGM_Resources.emplace(std::make_pair(name, tmp.release()));
#if _DEBUG
			std::cout << name << " audio bgm loaded\n";
#else
			std::string temp{ path.filename().string() + "bgm loaded\n" };
			SEWrite(temp);
#endif
			break;
		}
		case AUDIOTYPE::SFX:
		{
			std::unique_ptr<AudioResource> tmp = std::make_unique<AudioResource>(path);
			tmp->pAudio->isBGM = false;
			tmp->pAudio->isSFX = true;
			auto audioHandler = SEGetAudioHandle();
			auto res = audioHandler->system->createSound(path.string().c_str(), FMOD_LOOP_OFF, 0, &tmp->pAudio->track); (void)res;
			SFX_Resources.emplace(std::make_pair(name, tmp.release()));

#if _DEBUG
			std::cout << name << " audio sfx loaded\n";
#else
			std::string temp{ path.filename().string() + " sfx loaded\n" };
			SEWrite(temp);
#endif
			break;
		}
		}
	}
}


SEFont SEGetFont(std::filesystem::path name)
{
	auto search = Font_Resources.find(name.filename().string());
	if (search != Font_Resources.end())
	{
		return Font_Resources.at(name.filename().string())->font;
	}
	return {};
}

SEAudio* SEGetAudio(AUDIOTYPE AudioType, std::filesystem::path name)
{
	auto search_bgm = BGM_Resources.find(name.filename().string());
	auto search_sfx = SFX_Resources.find(name.filename().string());

	if (AudioType == AUDIOTYPE::BGM)
	{
		if (search_bgm->second)
			return search_bgm->second->pAudio.get();
	}
	else if (AudioType == AUDIOTYPE::SFX)
	{
		if (search_sfx->second)
			return search_sfx->second->pAudio.get();
	}
	return 0;
}

SEAudio* SEGetAudio(AUDIOTYPE AudioType, std::string track)
{
	auto search_bgm = BGM_Resources.find(track);
	auto search_sfx = SFX_Resources.find(track);

	if (AudioType == AUDIOTYPE::BGM)
	{
		if (search_bgm != BGM_Resources.end())
		{
			if (search_bgm->second)
				return search_bgm->second->pAudio.get();
		}
	}
	if (AudioType == AUDIOTYPE::SFX)
	{
		if (search_sfx != SFX_Resources.end())
			if (search_sfx->second)
				return search_sfx->second->pAudio.get();
	}
	return 0;
}

std::vector<std::string> SEGetTexturesResources_FileAndPath()
{
	std::vector<std::string> tmp;
	for (auto& texture : Texture_Resources)
	{
		tmp.push_back(texture.second->m_filename.string());
	}
	return tmp;
}

std::vector<std::string> SEGetFontResources_FileAndPath()
{
	std::vector<std::string> tmp;
	for (auto& font : Font_Resources)
	{
		tmp.push_back(font.second->m_filename.string());
	}
	return tmp;
}

const std::unordered_map<std::string, TextureResource*>& SEGetTextureResources()
{
	return Texture_Resources;
}

const std::unordered_map<std::string, FontResource*>& SEGetFontResources()
{
	return Font_Resources;
}

const std::unordered_map<std::string, AudioResource*>& SEGetAudioBGMResources()
{
	return BGM_Resources;
}

const std::unordered_map<std::string, AudioResource*>& SEGetAudioSFXResources()
{
	return SFX_Resources;
}



void SEResourceMgrExit()
{
	for (auto& texture : Texture_Resources)
	{
		if (texture.second)
			delete texture.second;
	}
	for (auto& font : Font_Resources)
	{
		if (font.second)
			delete font.second;
	}
	for (auto& bgm : BGM_Resources)
	{
		if (bgm.second)
			delete bgm.second;
	}
	for (auto& sfx : SFX_Resources)
	{
		if (sfx.second)
			delete sfx.second;
	}
	SEWrite("Resources cleared\n");
}

float GetTextureAr(const std::string& name) {
	float ar{};
	auto& map = SEGetTextureResources();
	auto i = map.find(name);
	if (i != map.end())
	{
		ar = (float)i->second->data->width / (float)i->second->data->height;
		//std::cout << "invalid aspect ratio" << i->second->data->width << " by " << i->second->data->width << std::endl;
		//assert(ar, "Invalid aspect Ratio!");						// width/height cannot be <= 0, otherwise throw assert.
		return ar;
	}

	return -1.0f;		// invalid AR
}