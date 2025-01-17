#pragma once
class Asset
{
public:
	bool AssetLoaded;
public:
	void Load(const char* Path);
	void Unload();
};

