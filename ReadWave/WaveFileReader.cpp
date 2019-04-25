#include "WaveFileReader.hpp"

#include <fstream>
#include <iostream>

WaveFileReader::WaveFileReader()
{
}

WaveFileReader::WaveFileReader(const std::string& filename)
	: filename_{filename}
{
}

std::string WaveFileReader::getFileName()
{
	return filename_;
}

void WaveFileReader::setFileName(const std::string& filename)
{
	filename_ = filename;
	header_read_ = false;
	data_read_ = false;
	data_.clear();
}

WaveFileFormat WaveFileReader::getHeader()
{
	if (header_read_) return header_;

	if (filename_.empty() || !readHeader()) 
		return WaveFileFormat{};

	return header_;
}

std::vector<double> WaveFileReader::getData()
{
	if (data_read_) return data_;

	if (filename_.empty() || !header_read_ && !readHeader())
		return std::vector<double>();

	std::ifstream filestream(filename_);
	if (!filestream.is_open()) {
		return std::vector<double>();
	}

	filestream.seekg(40); // пропускаем заголовок

	if (header_.fmt.wBitsPerSample != 8 && header_.fmt.wBitsPerSample != 16 && header_.fmt.wBitsPerSample != 32) {
		return std::vector<double>();
	}

	const int samples_count = header_.dataSubChunk.dataSizeInBytes / (header_.fmt.wBitsPerSample / 8);
	data_.reserve(samples_count);
	switch (header_.fmt.wBitsPerSample)
	{
	case 8: {
		for (int i = 0; i < samples_count; ++i) {
			std::int8_t buffer{};
			filestream.read(reinterpret_cast<char*>(&buffer), sizeof(decltype(buffer)));
			data_.push_back(buffer == 0 ? 0.0 : 1.0 / buffer);
		}
	} break;
	case 16: {
		for (int i = 0; i < samples_count; ++i) {
			std::int16_t buffer{};
			filestream.read(reinterpret_cast<char*>(&buffer), sizeof(decltype(buffer)));
			data_.push_back(buffer == 0 ? 0.0 : 1.0 / buffer);
		}
	} break;
	case 32: {
		for (int i = 0; i < samples_count; ++i) {
			std::int32_t buffer{};
			filestream.read(reinterpret_cast<char*>(&buffer), sizeof(decltype(buffer)));
			data_.push_back(buffer == 0 ? 0.0 : 1.0 / buffer);
		}
	} break;
	default:
		return std::vector<double>();
	}

	filestream.close();
	data_read_ = true;
	return data_;
}

bool WaveFileReader::readHeader()
{
	std::ifstream filestream(filename_);
	if (!filestream.is_open()) {
		return false;
	}

	filestream.read(header_.ckID.data(), header_.ckID.size() - 1);
	filestream.read(reinterpret_cast<char*>(&(header_.cksize)),
		sizeof(decltype(header_.cksize)));
	filestream.read(header_.WAVEID.data(), header_.WAVEID.size() - 1);

	filestream.read(header_.fmt.ckID.data(), header_.fmt.ckID.size() - 1);
	filestream.read(reinterpret_cast<char*>(&header_.fmt.cksize),
		sizeof(decltype(header_.fmt.cksize)));
	filestream.read(reinterpret_cast<char*>(&header_.fmt.wFormatTag),
		sizeof(decltype(header_.fmt.wFormatTag)));
	filestream.read(reinterpret_cast<char*>(&header_.fmt.nChannels),
		sizeof(decltype(header_.fmt.nChannels)));
	filestream.read(reinterpret_cast<char*>(&header_.fmt.nSamplesPerSec),
		sizeof(decltype(header_.fmt.nSamplesPerSec)));
	filestream.read(reinterpret_cast<char*>(&header_.fmt.nAvgBytesPerSec),
		sizeof(decltype(header_.fmt.nAvgBytesPerSec)));
	filestream.read(reinterpret_cast<char*>(&header_.fmt.nBlockAlign),
		sizeof(decltype(header_.fmt.nBlockAlign)));
	filestream.read(reinterpret_cast<char*>(&header_.fmt.wBitsPerSample),
		sizeof(decltype(header_.fmt.wBitsPerSample)));
	filestream.read(header_.dataSubChunk.dataID.data(), header_.dataSubChunk.dataID.size() - 1);
	filestream.read(reinterpret_cast<char*>(&header_.dataSubChunk.dataSizeInBytes),
		sizeof(decltype(header_.dataSubChunk.dataSizeInBytes)));

	filestream.close();
	header_read_ = true;
	return true;
}
