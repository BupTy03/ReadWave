#include "WaveFileReader.hpp"

#include <iostream>
#include <vector>
#include <string>

template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec)
{
	os << "[ ";
	for (auto&& v : vec) os << v << " ";
	os << ']';
	return os;
}

int main(int argc, char* argv[])
{
	using std::cout;
	using std::endl;

	WaveFileReader reader("bell1.wav");
	auto header = reader.getHeader(); // file header

	cout << "Chunk id: " << header.ckID.data() << endl;
	cout << "Chunk size: " << header.cksize << endl;
	cout << "Format: " << header.WAVEID.data() << endl;

	cout << "\nfmt chunk: \n";
	cout << "Chunk id: " << header.fmt.ckID.data() << endl;
	cout << "Chunk size: " << header.fmt.cksize << endl;
	cout << "Format tag: " << header.fmt.cksize << endl;
	cout << "Number channels: " << header.fmt.nChannels << endl;
	cout << "Samples/sec: " << header.fmt.nSamplesPerSec << endl;
	cout << "Averange bytes/sec: " << header.fmt.nAvgBytesPerSec << endl;
	cout << "Block align: " << header.fmt.nBlockAlign << endl;
	cout << "Bits/sample: " << header.fmt.wBitsPerSample << endl;
	
	cout << "\ndata chunk: \n";
	cout << "Data letters: " << header.dataSubChunk.dataID.data() << endl;
	cout << "Size of data in bytes: " << header.dataSubChunk.dataSizeInBytes << endl;

	auto data = reader.getData(); // file data

	cout << "\nData: \n";
	cout << data << endl;

	return 0;
}