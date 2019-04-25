#pragma once
#ifndef WAVE_FILE_READER
#define WAVE_FILE_READER

#include <vector>
#include <array>

struct FmtChunk
{
	std::array<char, 5> ckID{'\0'};
	std::int32_t cksize{};
	std::int16_t wFormatTag{};
	std::int16_t nChannels{};
	std::int32_t nSamplesPerSec{};
	std::int32_t nAvgBytesPerSec{};
	std::int16_t nBlockAlign{};
	std::int16_t wBitsPerSample{};
};

struct DataSubChunk
{
	std::array<char, 5> dataID{ '\0' };
	std::int32_t dataSizeInBytes{};
};

struct WaveFileFormat
{
	std::array<char, 5> ckID{ '\0' };
	std::int32_t cksize{};
	std::array<char, 5> WAVEID{ '\0' };
	FmtChunk fmt;
	DataSubChunk dataSubChunk;
};

struct WaveFileReader
{
	WaveFileReader();
	WaveFileReader(const std::string& filename);

	std::string getFileName();
	void setFileName(const std::string& filename);

	WaveFileFormat getHeader();
	std::vector<double> getData();

private:
	bool readHeader();

private:
	bool header_read_{ false };
	bool data_read_{ false };
	std::string filename_;
	WaveFileFormat header_;
	std::vector<double> data_;
};

#endif // !WAVE_FILE_READER
