#include "netpbmloader.h"


s_NetPBMFileDesc* NetPBMLoader::read(std::string filename, std::string ext) {
    s_NetPBMFileDesc* fileDesc = new s_NetPBMFileDesc();
    std::string path = MEDIA_PATH + filename + ext;
    std::string line1;
    std::string line2;
    std::ifstream file(path.c_str(), std::ios::in | std::ios::binary);
    std::ifstream::pos_type start, end, size;
    std::ifstream::off_type newStart;
    
    if(!file) {
        throw std::runtime_error("Can't open file" + path);
    }   

    std::cout << "Reading " << path << std::endl;

    getline(file, line1);
    start = file.tellg();
    getline(file, line2);

    std::cout << "Header: " << std::endl;
    std::cout << line1 << std::endl;

    if(line2[0] != '#') {
        file.seekg(start, std::ios::beg);
    } else {
        std::cout << line2 << std::endl;
    }

    file >> fileDesc->width;
    file >> fileDesc->length;
    file >> fileDesc->colors;

    std::cout << " - Width: " << fileDesc->width << ", Length: " << fileDesc->length << std::endl;
    std::cout << " - Colors: " << fileDesc->colors << std::endl;

    start = file.tellg();
    file.seekg (0, std::ios::end);
    end = file.tellg();
    size = end - start;

    char* memblock = new char[size];
    newStart = static_cast<std::ifstream::off_type>(start);
    file.seekg(newStart+1, std::ios::beg);
    file.read(memblock, size);
    file.close();

    fileDesc->memblock = (unsigned char*)memblock;

    return fileDesc;
}

PGMImage NetPBMLoader::loadPGM(std::string filename) { 
    s_NetPBMFileDesc* fileDesc = read(filename, PGM_EXT);   
    PGMImage image(fileDesc->length, fileDesc->width);

    for(int i = 0; i < image.getLength(); ++i) {
        for(int j = 0; j < image.getWidth(); ++j) {
            image(i, j) = (float)(fileDesc->memblock[i*image.getLength()+j]);
        }   
    }

    delete[] fileDesc->memblock;
    delete fileDesc;

    return image;
}

PPMImage NetPBMLoader::loadPPM(std::string filename) {
    s_NetPBMFileDesc* fileDesc = read(filename, PPM_EXT);
    PPMImage image(fileDesc->length, fileDesc->width);

    for(int i = 0; i < image.getLength(); ++i) {
        for(int j = 0; j < image.getWidth() * 3; j+=3) {
            Vector3D color;
            color.r = (float)(fileDesc->memblock[i*image.getLength()*3+j]);
            color.g = (float)(fileDesc->memblock[i*image.getLength()*3+j+1]);
            color.b = (float)(fileDesc->memblock[i*image.getLength()*3+j+2]);
            image(i, j/3) = color;
        }   
    }

    delete[] fileDesc->memblock;
    delete fileDesc;

    return image;
}


void NetPBMLoader::savePGM(PGMImage& pgmimage, std::string filename) {
    std::string path = MEDIA_PATH + filename + PGM_EXT;
    std::ofstream file(path.c_str(), std::ios::out | std::ios::binary);

    if(file.is_open()) {
        file << "P5\n";
        file << pgmimage.getLength() << " " << pgmimage.getWidth() << "\n";
        file << COLOR_LEVELS << "\n";

        for(int i = 0; i < pgmimage.getLength(); ++i) {
            for(int j = 0; j < pgmimage.getWidth(); ++j) {
                file << (char)(pgmimage(i, j));
            }
        }

        file.close();
        std::cout << "Saved file " << path << std::endl;
    }
}

void NetPBMLoader::savePPM(PPMImage& pgmimage, std::string filename) {
    std::string path = MEDIA_PATH + filename + PPM_EXT;
    std::ofstream file(path.c_str(), std::ios::out | std::ios::binary);

    if(file.is_open()) {
        file << "P6\n";
        file << pgmimage.getLength() << " " << pgmimage.getWidth() << "\n";
        file << COLOR_LEVELS << "\n";

        for(int i = 0; i < pgmimage.getLength(); ++i) {
            for(int j = 0; j < pgmimage.getWidth(); ++j) {
                file << (char)(pgmimage(i, j).r);
                file << (char)(pgmimage(i, j).g);
                file << (char)(pgmimage(i, j).b);
            }
        }
        std::cout << pgmimage(0,0).r;
        std::cout << " " << pgmimage(0,0).g;
        std::cout << " " << pgmimage(0,0).b;
        std::cout << std::endl;

        file.close();
        std::cout << "Saved file " << path << std::endl;
    }
}

