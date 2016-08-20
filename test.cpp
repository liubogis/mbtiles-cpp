#include <iostream>
#include <sqlite3.h>
#include <string>
#include <cstring>
#include <map>
#include <sstream>
#include "ReadGzip.h"
using namespace std;
#include "MBTileReader.h"
#include "vector_tile20/vector_tile.pb.h"

#include <string>
#include <sstream>
#include <vector>

using namespace std;

// http://stackoverflow.com/a/236803/4288232
void strsplit(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

int main(int argc, char **argv)
{
	class MBTileReader mbTileReader("cairo_egypt.mbtiles");	
	
	cout << "name:" << mbTileReader.GetMetadata("name") << endl;
	cout << "type:" << mbTileReader.GetMetadata("type") << endl;
	string version = mbTileReader.GetMetadata("version");
	cout << "version:" << version << endl;
	vector<string> versionSplit;
	strsplit(version, '.', versionSplit);
	vector<int> versionInts;
	for (int i=0;i<versionSplit.size();i++) versionInts.push_back(atoi(versionSplit[i].c_str()));
	cout << "description:" << mbTileReader.GetMetadata("description") << endl;
	string format = mbTileReader.GetMetadata("format");
	cout << "format:" << format << endl;
	cout << "bounds:" << mbTileReader.GetMetadata("bounds") << endl;

	if(0) //Get metadata fields
	{
		std::vector<std::string> fieldNames;
		mbTileReader.GetMetadataFields(fieldNames);
		for(unsigned i=0;i<fieldNames.size();i++) cout << fieldNames[i] << endl;
	}

	if(0) //Get list of tiles
	{
		TileInfoRows tileInfoRows;
		mbTileReader.ListTiles(tileInfoRows);
		for(unsigned i=0;i < tileInfoRows.size(); i++)
		{
			for(size_t j=0; j < tileInfoRows[i].size(); j++)
				cout << tileInfoRows[i][j] << ",";
			cout << endl;
		}
	}

	string blob;
	mbTileReader.GetTile(14,9618,9611,blob);

	if(format == "pbf" && versionInts[0] == 2)
	{
		std::stringbuf buff;
		buff.sputn(blob.c_str(), blob.size());
		DecodeGzip dec(buff);

		string tileData;

		char tmp[1024];
		while(dec.in_avail())
		{
			streamsize bytes = dec.sgetn(tmp, 1024);
			tileData.append(tmp, bytes);
		}

		vector_tile::Tile tile;
		cout << "ParseFromString: " << tile.ParseFromString(tileData) << endl;
		cout << "Num layers: " << tile.layers_size() << endl;
	}
}

